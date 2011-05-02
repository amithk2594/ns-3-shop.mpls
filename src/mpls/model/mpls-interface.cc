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
#include "ns3/ipv4.h"

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

Interface::Interface ()
  : m_mpls (0),
    m_device (0),
    m_ipv4if (-1),
    m_ifup (false),
    m_ifIndex (0),
    m_addressResolvingMode (AUTO)
{
}

Interface::~Interface ()
{
}

void
Interface::DoDispose (void)
{
  m_mpls = 0;
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
  NS_ASSERT_MSG (m_labelSpace.IsEmpty (), "Clear interface label space before set new range");
  m_labelSpace.SetMinValue (value);
}

void
Interface::SetMaxLabelValue (uint32_t value)
{
  NS_ASSERT_MSG (m_labelSpace.IsEmpty (), "Clear interface label space before set new range");
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
Interface::SetMpls (const Ptr<Mpls> &mpls)
{
  m_mpls = mpls;
}

Ptr<Mpls>
Interface::GetMpls (void) const
{
  return m_mpls;
}

void
Interface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
}

void 
Interface::SetIfIndex (uint32_t index)
{
  m_ifIndex = index;
}

Ptr<NetDevice>
Interface::GetDevice (void) const
{
  return m_device;
}

uint32_t
Interface::GetIfIndex (void) const
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
  m_ifup = true;
}

void
Interface::SetDown ()
{
  m_ifup = false;
}

void
Interface::Send (const Ptr<Packet>& packet, const Mac48Address &nextHop)
{
  m_device->Send (packet, nextHop, Mpls::PROT_NUMBER);
}

int32_t
Interface::LookupIpv4Interface (void)
{
  if (m_ipv4if >= 0) 
    {
      return m_ipv4if;
    }

  Ptr<Ipv4> ipv4 = m_mpls->GetIpv4 ();
  if (ipv4 != 0)
    {
      m_ipv4if = ipv4->GetInterfaceForDevice (m_device);
    }

  return m_ipv4if;
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
