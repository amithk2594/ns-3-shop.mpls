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

#ifndef MPLS_PROTOCOL_INFORMATION_H
#define MPLS_PROTOCOL_INFORMATION_H

#include <ostream>
#include <list>

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"

#define ENSURE_REGISTER_PIO(type)                             \
  static struct _MPLS_PIO_##type##_RegistrationClass          \
  {                                                           \
    _MPLS_PIO_##type##_RegistrationClass () {                 \
        ProtocolInformationObject::Register (                 \
          type::PROTO,                                        \
          MakeCallback(&type::Factory)                        \
        );                                                    \
    }                                                         \
  } _mpls_pio_##type##_RegistrationVariable

namespace ns3 {
namespace mpls {

class ProtocolInformationObject;

class ProtocolInformationContext
{
public:
  ProtocolInformationContext (const Ptr<const Packet> &packet, const Ipv4Header &header);
  //ProtocolInformationContext (const Ptr<const Packet> &packet, const Ipv6Header &header);
  ~ProtocolInformationContext ();
  
  const ProtocolInformationObject* GetInfo (uint32_t id) const;
  void AddInfo (uint32_t id, const ProtocolInformationObject* obj);
  
private:
  typedef std::map<uint32_t, ProtocolInformationObject> ContextMap;
  Ptr<Packet> m_packet;
  ContextMap m_map;
};


class ProtocolInformationObject
{
public:
  ProtocolInformationObject ();
  virtual ~ProtocolInformationObject ();
};


class Ipv4ProtocolInformation: public ProtocolInformationObject
{
public:
  Ipv4ProtocolInformation (const Ipv4Header &header);
  virtual ~Ipv4ProtocolInformation ();

  // from Ipv4Header
  uint16_t GetIdentification (void) const;
  uint8_t GetTos (void) const;
  uint8_t GetTtl (void) const;
  uint8_t GetProtocol (void) const;
  const Ipv4Address& GetSource (void) const;
  const Ipv4Address& GetDestination (void) const;

private:
  uint16_t m_identification;
  uint8_t m_tos;
  uint8_t m_ttl;
  uint32_t m_protocol;
  Ipv4Address m_source;
  Ipv4Address m_destination;
};


} // namespace mpls
} // namespace ns3

#endif /* MPLS_PROTOCOL_INFORMATION_H */
