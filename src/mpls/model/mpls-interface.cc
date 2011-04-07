/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrey Churin <aachurin@gmail.com>
 *         Stefano Avallone <stavallo@gmail.com>
 */

#define NS_LOG_APPEND_CONTEXT \
    if (m_node != 0) { \
      std::clog << Simulator::Now ().GetSeconds () \
      << " [node " << m_node->GetId () << "] "; }

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

#include "mpls.h"
#include "mpls-interface.h"

NS_LOG_COMPONENT_DEFINE ("mpls::Interface");

namespace ns3 {
namespace mpls {

TypeId
Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Interface")
    .SetParent<Object> ()
    .AddAttribute ("AddressResolvingMode", 
                   "The link-layer address resolving mode.",
                   EnumValue (AUTO),
                   MakeEnumAccessor (&Interface::SetAddressResolvingMode, 
                                     &Interface::GetAddressResolvingMode),
                   MakeEnumChecker (AUTO, "Auto",
                                    STATIC, "Static",
                                    DYNAMIC, "Dynamic"))    
    ;
  return tid;
}

Interface::Interface (int32_t ifIndex)
  : m_node (0),
    m_device (0),
    m_cache (0),
    m_ifup (true),
    m_ifIndex (ifIndex),
    m_addressResolvingMode (AUTO)
{
  NS_LOG_FUNCTION (this);
}

Interface::~Interface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Interface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void
Interface::SetNode (const Ptr<Node> &node)
{
  m_node = node;
  DoSetup ();
}

void
Interface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
  DoSetup ();  
}

void
Interface::SetArpCache (const Ptr<ArpCache> &cache)
{
  m_cache = cache;
}

void
Interface::DoSetup ()
{
  if (m_node == 0 || m_device == 0)
    {
      return;
    }
}

Ptr<NetDevice>
Interface::GetDevice (void)
{
  return m_device;
}

int32_t
Interface::GetIfIndex (void)
{
  return m_ifIndex;
}
  
bool
Interface::IsUp () const
{
  return m_ifup;
}

bool
Interface::IsDown () const
{
  return !m_ifup;
}

void
Interface::SetUp ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = true;
}

void
Interface::SetDown ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = false;
}

void
Interface::Send (const Ptr<Packet>& packet, const Address &dest)
{
  if (Mac48Address::IsMatchingType (dest))
    {
      // I believe you know what are you doing      
      m_device->Send (packet, Mac48Address::ConvertFrom (dest), Mpls::PROT_NUMBER);
    }
  else if (!m_device->NeedsArp()) 
    {
      m_device->Send (packet, m_device->GetBroadcast (), Mpls::PROT_NUMBER);
    }
  else if (Ipv4Address::IsMatchingType (dest))
    {
      Mac48Address* hwaddr = m_addressResolvingTable->Lookup (Ipv4Address::ConvertFrom (dest))

//      NS_LOG_DEBUG ("MplsInterface: needs ARP for " << destination);

//      ipv4if = GetObject<Ipv4Interface> ();
//      
//      NS_ASSERT_MSG (ipv4if != 0, "MplsInterface: there is no associated ipv4-interface");
      
    }
  else 
    {
      NS_ASSERT_MSG (false,
                     "MplsInterface: node " << m_node->GetId() << " ifIndex " << m_ifIndex << 
                     ", needs ARP -- specify next hop address!");    
    }
}

void
Interface::Send (const Ptr<Packet>& packet, const Mac48Address &nextHop)
{
  m_device->Send (packet, nextHop, Mpls::PROT_NUMBER);
}

const Mac48Address*
Interface::LookupAddress (Ipv4Address& destination)
{
  Ipv4TableIterator i = m_ipv4AddressResolvingTable.find (destination);
  
  if (i == m_ipv4AddressResolvingTable.end ())
    return 0;
  
  return &(i->second);
}

void
Interface::AddAddress (const Ipv4Address& dest, const Mac48Address& mac)
{
  m_ipv4AddressResolvingTable.insert (std::pair<Ipv4Address, Mac48Address> (dest, mac));
}

void
Interface::RemoveAddress (const Ipv4Address& dest)
{
  m_ipv4AddressResolvingTable.erase (dest);
}


} // namespace mpls
} // namespace ns3
