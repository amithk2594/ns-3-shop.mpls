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

#ifndef MPLS_PACKET_DEMUX_H
#define MPLS_PACKET_DEMUX_H

#include <ostream>

#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"

namespace ns3 {
namespace mpls {

class HeaderHolder
{
public:
  HeaderHolder (Header* header);
  ~HeaderHolder ();
  Header* Get () const;
  void UnSet (void);
  void Reset (void);
  bool IsReset (void) const;
  Header* SetFromPacket (const Ptr<Packet>& packet);  
  
protected:
  enum State {
    RESET = 0,
    SET,    
    UNSET
  };

  enum State m_state;

  Header* m_header;
};

/**
 * \ingroup Mpls
 * \brief
 * Packet context which holds common headers for Ipv4 and Ipv6
 */
class PacketDemux
{
public:
  PacketDemux ();
  ~PacketDemux ();
  
  /**
   * @brief Assign new packet and Ipv4 header.
   */
  void Assign (const Ptr<const Packet> &packet, const Ipv4Header &header);
  /**
   * @brief Assign new packet and Ipv6 header.
   */
  void Assign (const Ptr<const Packet> &packet, const Ipv6Header &header);  
  /**
   * @brief Release
   */  
  void Release (void);    
  /**
   * \return Ipv4 header if exists
   */
  const Ipv4Header* GetIpv4Header (void);
  /**
   * \return Ipv4 header if exists
   */
  const Ipv6Header* GetIpv6Header (void);  
  /**
   * \return Udp header if exists
   */
  const UdpHeader* GetUdpHeader (void);
  /**
   * \return Tcp header if exists
   */
  const TcpHeader* GetTcpHeader (void);
  
private:
  Header* GetHeaderFromHolder (uint8_t protocol, HeaderHolder& holder);

  Ptr<Packet> m_packet;
  const Ipv4Header* m_ipv4Header;
  const Ipv4Header* m_ipv6Header;  
  HeaderHolder m_tcpHolder;
  HeaderHolder m_udpHolder;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_PACKET_DEMUX_H */
