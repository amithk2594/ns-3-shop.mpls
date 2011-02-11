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
#include "mpls-protocol-information.h"

namespace ns3 {
namespace mpls {

ProtocolInformationContext::ProtocolInformationContext (const Ptr<const Packet> &packet, const Ipv4Header &header)
{
  m_packet = packet->Copy ();
  Ipv4ProtocolInformation* info = new Ipv4ProtocolInformation (header);
  AddInfo (Ipv4ProtocolInformation::GetInfoId(), info);
}

//ProtocolInformationContext::ProtocolInformationContext (const Ptr<const Packet> &packet, const Ipv6Header &header)
//{
//  m_packet = packet->Copy ();
//  Ipv6ProtocolInformation* info = new Ipv6ProtocolInformation (header);
//  AddInfo (Ipv6ProtocolInformation::GetInfoId(), info);
//}


ProtocolInformationContext::~ProtocolInformationContext ()
{
  for (ContextMap::iterator i = m_map.begin(); i != m_map.end(); ++i)
    {
      delete i->second;
    }

  m_map.clear ();
  m_packet = 0;
}
  
const ProtocolInformationObject* 
ProtocolInformationContext::Get (uint32_t id) const
{
  ContextMap::iterator it = m_map.find (id);
  return it != m_map.end() ? it->second : 0;
}

void 
ProtocolInformationContext::Add (uint32_t id, const ProtocolInformationObject* obj)
{
  m_map[id] = obj;
}
  

ProtocolInformationObject::ProtocolInformationObject ()
{
}

ProtocolInformationObject::~ProtocolInformationObject ()
{
}


Ipv4ProtocolInformation::Ipv4ProtocolInformation (const Ipv4Header &header)
{
  m_identification = header.GetIdentification ();
  m_tos = header.GetTos ();
  m_ttl = header.GetTtl ();
  m_protocol = header.GetProtocol ();
  m_source = header.GetSource ();
  m_destination = header.GetDestination ();
}

Ipv4ProtocolInformation::~Ipv4ProtocolInformation ()
{
}

uint16_t
Ipv4ProtocolInformation::GetIdentification (void) const
{
  return m_identification;
}

uint8_t
Ipv4ProtocolInformation::GetTos (void) const
{
  return m_tos;
}

uint8_t
Ipv4ProtocolInformation::GetTtl (void) const
{
  return m_ttl;
}

uint8_t
Ipv4ProtocolInformation::GetProtocol (void) const
{
  return m_protocol;
}

const Ipv4Address& 
Ipv4ProtocolInformation::GetSource (void) const
{
  return m_source;
}

const Ipv4Address& 
Ipv4ProtocolInformation::GetDestination (void) const
{
  return m_destination;
}


} // namespace mpls
} // namespace ns3
