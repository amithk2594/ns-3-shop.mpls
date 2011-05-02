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
#include "ns3/enum.h"
#include "ns3/uinteger.h"

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
    .AddAttribute ("MinLabelValue", 
                   "The minimum label value.",
                   UintegerValue (0x1000),
                   MakeUintegerAccessor (&Interface::SetMinLabelValue),
                   MakeUintegerChecker<uint32_t> (0x10, 0xfffff))
    .AddAttribute ("MaxLabelValue", 
                   "The maximum label value.",
                   UintegerValue (0xfffff),
                   MakeUintegerAccessor (&Interface::SetMaxLabelValue),
                   MakeUintegerChecker<uint32_t> (0x10, 0xfffff))
    ;
  return tid;
}

Interface::Interface (int32_t ifIndex)
  : m_node (0),
    m_device (0),
    m_ifup (true),
    m_ifIndex (ifIndex),
    m_addressResolvingMode (AUTO)
{
  NS_LOG_FUNCTION (this);
}

Interface::~Interface ()
{
  NS_LOG_FUNCTION (this);
}

void
Interface::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

LabelSpace*
Interface::GetLabelSpace (void)
{
  return &m_labelSpace;
}

void
Interface::SetMinLabelValue (uint32_t value)
{
  NS_ASSERT_MSG (!m_labelSpace.IsUsed (), "Clear interface label space before set new range");
  m_labelSpace.SetMinValue (value);
}

void
Interface::SetMaxLabelValue (uint32_t value)
{
  NS_ASSERT_MSG (!m_labelSpace.IsUsed (), "Clear interface label space before set new range");
  m_labelSpace.SetMaxValue (value);
}
  
void
Interface::SetAddressResolvingMode (Interface::AddressResolvingMode mode)
{
  m_addressResolvingMode = mode;
}

Interface::AddressResolvingMode
Interface::GetAddressResolvingMode (void) const
{
  return m_addressResolvingMode;
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
Interface::DoSetup ()
{
  if (m_node == 0 || m_device == 0)
    {
      return;
    }
}

Ptr<NetDevice>
Interface::GetDevice (void) const
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
   m_device->Send (packet, nextHop, Mpls::PROT_NUMBER);
}

bool
Interface::LookupAddress (const Address& dest, Mac48Address& hwaddr)
{
  if (Ipv4Address::IsMatchingType (dest))
    {
      Ipv4TableIterator i = m_ipv4resolving.find (Ipv4Address::ConvertFrom (dest));
  
      if (i != m_ipv4resolving.end ())
        {
          hwaddr = (*i).second;
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
      m_ipv4resolving[Ipv4Address::ConvertFrom (dest)] = mac;
    }
}

void
Interface::RemoveAddress (const Address& dest)
{
  if (Ipv4Address::IsMatchingType (dest))
    {
      m_ipv4resolving.erase (Ipv4Address::ConvertFrom (dest));
    }
}

void 
Interface::RemoveAllAddresses (void)
{
  m_ipv4resolving.clear ();
}

} // namespace mpls
} // namespace ns3
