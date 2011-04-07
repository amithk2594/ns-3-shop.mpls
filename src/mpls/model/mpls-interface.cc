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
Interface::Send (const Ptr<Packet>& packet, const Mac48Address &nextHop)
{
  if (!m_device->NeedsArp()) 
    {
      m_device->Send (packet, m_device->GetBroadcast (), Mpls::PROT_NUMBER);
    }
  else
    {
      m_device->Send (packet, nextHop, Mpls::PROT_NUMBER);
    }
}

bool
Interface::LookupAddress (const Ipv4Address& dest, Mac48Address& mac)
{
  if (Ipv4Address::IsMatchingType (dest))
    {
      Ipv4TableIterator i = m_ipv4resolving.find (Ipv4Address::ConvertFrom (dest));
  
      if (i != m_ipv4resolving.end ())
        {
          mac = i.second;
          return 1;
        }
    }
        
  return 0;
}

void
Interface::AddAddress (const Address& dest, const Mac48Address& mac)
{
  if (Ipv4Address::IsMatchingType (dest))
    {
      m_ipv4resolving.insert (std::pair<Ipv4Address, Mac48Address> (Ipv4Address::ConvertFrom (dest), mac));
    }
}

void
Interface::RemoveAddress (const Ipv4Address& dest)
{
  if (Ipv4Address::IsMatchingType (dest))
    {
      m_ipv4resolving.erase (Ipv4Address::ConvertFrom (dest));
    }
}

} // namespace mpls
} // namespace ns3
