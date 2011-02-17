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

#ifndef MPLS_PACKET_INFORMATION_H
#define MPLS_PACKET_INFORMATION_H

#include <ostream>
#include <list>

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * Packet information
 */
class PacketContext
{
public:
  PacketContext (const Ptr<const Packet> &packet, const Ipv4Header &header);
  PacketContext (const Ptr<const Packet> &packet, const Ipv6Header &header);
  ~PacketContext ();
  
  /**
   * \param typeId 
   * \returns ProtocolInformation or 0
   */
  const ProtocolInformation* GetInfo (uint32_t typeId);
  // void AddInfo (uint32_t id, const ProtocolInformationObject* obj);

private:
  static FactoryVector& GetRegistredFactories (void);

  typedef Callback <ProtocolInformation*, PacketContext*> FactoryCallback;
  typedef std::vector<FactoryCallback> FactoryVector;
  typedef std::map<uint32_t, ProtocolInformation*> ContextMap;
  ContextMap m_map;
  Ptr<Packet> m_packet;
};

class ProtocolInformation
{
public:
  ProtocolInformation ();
  virtual ~ProtocolInformation ();
};

class Ipv4Info: public ProtocolInformation
{
public:
  static uint32_t GetType (void);

  Ipv4Info (const Ipv4Header &header);
  virtual ~Ipv4Info ();

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

class Ipv6Info: public ProtocolInformation
{
public:
  static uint32_t GetType (void);

  Ipv6Info (const Ipv6Header &header);
  virtual ~Ipv6Info ();
};

class UdpInfo: public ProtocolInformation
{
public:
  static uint32_t GetType (void);
  static ProtocolInformation* Factory (PacketContext* context);

  UdpInfo (const UdpHeader &header);
  virtual ~UdpInfo ();

  // from UdpHeader
  uint16_t GetSourcePort (void) const;
  uint16_t GetDestinationPort (void) const;

private:
  uint16_t m_sourcePort;
  uint16_t m_destinationPort;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_PROTOCOL_INFORMATION_H */
