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

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * Packet information context
 */
class PacketContext
{
public:
  PacketContext (const Ptr<const Packet> &packet, const Ipv4Header &header);
  PacketContext (const Ptr<const Packet> &packet, const Ipv6Header &header);
  ~PacketContext ();

  /**
   * Get Ipv4 header if exists
   */
  const Ipv4Header* GetIpv4 (void); 
  /**
   * Get Ipv6 header if exists
   */
  const Ipv6Header* GetIpv6 (void);
  /**
   * Get Udp header if exists
   */
  const UdpHeader* GetUdp (void);
  /**
   * Get Tcp header if exists
   */
  const TcpHeader* GetTcp (void);

private:
  void ResolveUdpHeader (void);
  void ResolveUdpHeader (void);
//  void ResolveIcmpHeader (void);
// etc

private:  
  Ptr<Packet> m_packet;
  Ipv4Header* m_ipv4Header;
  Ipv6Header* m_ipv6Header;
  UdpHeader* m_udpHeader;
  bool m_udpResolved;
  TcpHeader* m_tcpHeader;
  bool m_tcpResolved;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_PROTOCOL_INFORMATION_H */
