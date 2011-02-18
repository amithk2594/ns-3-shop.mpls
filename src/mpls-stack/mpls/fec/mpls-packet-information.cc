/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
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
 */

#include <iomanip>
#include "mpls-packet-information.h"

namespace ns3 {
namespace mpls {

static
FactoryVector& 
GetRegistredFactories (void)
{
  static FactoryVector factories;
  return factories;
}

PacketContext::PacketContext (const Ptr<const Packet> &packet, const Ipv4Header &header)
{
  m_packet = packet->Copy ();
  Ipv4Info* info = new Ipv4Info (header);
  // ensure ipv4 and ipv6 information already exists
  m_map.insert(m_map.begin(), std::pair<uint32_t, ProtocolInformation*> (Ipv6Info::GetType (), 0));
  m_map.insert(m_map.begin(), std::pair<uint32_t, ProtocolInformation*> (Ipv4Info::GetType (), info));
}

PacketContext::PacketContext (const Ptr<const Packet> &packet, const Ipv6Header &header)
{
  m_packet = packet->Copy ();
  Ipv6Info* info = new Ipv6Info (header);
  m_map.insert(m_map.begin(), std::pair<uint32_t, ProtocolInformation*> (Ipv4Info::GetType (), 0));
  m_map.insert(m_map.begin(), std::pair<uint32_t, ProtocolInformation*> (Ipv6Info::GetType (), info));
}

PacketContext::~PacketContext ()
{
  // free allocated memory
  for (ContextMap::iterator i = m_map.begin(); i != m_map.end(); ++i)
    {
      if (i->second)
        {
          delete i->second;
        }
    }

  m_map.clear ();
  m_packet = 0;
}
  
const ProtocolInformation* 
PacketContext::GetInfo (uint32_t typeId)
{
  ContextMap::iterator it = m_map.find (typeId);
  if (it != m_map.end ())
    {
      return it->second;
    }
  
  FactoryVector& factories = GetRegistredFactories ();
  NS_ASSERT_MSG (typeId < factories.size(), 
      "PacketContext::GetInfo(): Object is not registred");

  ProtocolInformation* info = factories[typeId](&this);
  m_map.insert(m_map.begin(), std::pair<uint32_t, ProtocolInformation*> (typeId, info));
  return info;
}

ProtocolInformation::ProtocolInformation ()
{
}

ProtocolInformation::~ProtocolInformation ()
{
}

static 
uint32_t 
Ipv4Info::GetType (void)
{
  return 1;
}

Ipv4Info::Ipv4Info (const Ipv4Header &header)
{
  m_identification = header.GetIdentification ();
  m_tos = header.GetTos ();
  m_ttl = header.GetTtl ();
  m_protocol = header.GetProtocol ();
  m_source = header.GetSource ();
  m_destination = header.GetDestination ();
}

Ipv4Info::~Ipv4Info ()
{
}

uint16_t
Ipv4Info::GetIdentification (void) const
{
  return m_identification;
}

uint8_t
Ipv4Info::GetTos (void) const
{
  return m_tos;
}

uint8_t
Ipv4Info::GetTtl (void) const
{
  return m_ttl;
}

uint8_t
Ipv4Info::GetProtocol (void) const
{
  return m_protocol;
}

const Ipv4Address& 
Ipv4Info::GetSource (void) const
{
  return m_source;
}

const Ipv4Address& 
Ipv4Info::GetDestination (void) const
{
  return m_destination;
}

static 
uint32_t 
Ipv6Info::GetType (void)
{
  return 2;
}

Ipv6Info::Ipv6Info (const Ipv6Header &header)
{
}

Ipv6Info::~Ipv6Info ()
{
}

static 
uint32_t 
UdpInfo::GetType (void)
{
  static uint32_t id = PacketContext::Register (&Factory);
  return id;
}

static 
ProtocolInformation* 
UdpInfo::Factory (PacketContext* context)
{
  // check if ipv4
  Ipv4Info* ipv4info = context->GetInfo (Ipv4Info::GetType ());
  if (ipv4info)
    {
      if (ipv4info->GetProtocol () == 17) {
        UdpHeader udpHeader;
        context->GetPacket ()->RemoveHeader (udpHeader);
        return new UdpInfo (updHeader);
      }
      return 0;
    }
  // TODO: add ipv6 support here
  return 0;
}

UdpInfo::UdpInfo (const UdpHeader &header)
  : m_sourcePort (header.GetSourcePort ()),
    m_destinationPort (header.GetDestinationPort ())
{
}

UdpInfo::~UdpInfo ()
{
}

uint16_t 
GetSourcePort (void) const
{
  return m_sourcePort;
}

uint16_t 
GetDestinationPort (void) const
{
  return m_destinationPort;
}

} // namespace mpls
} // namespace ns3
