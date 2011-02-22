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
 * Packet context which holds common headers for Ipv4 and Ipv6
 */
class CommonPacketData
{
public:
  virtual ~CommonPacketData ();

public:
  /**
   * \return Ipv4 header if exists
   */
  virtual const Ipv4Header* GetIpv4Header (void);
  /**
   * \return Ipv6 header if exists
   */
  virtual const Ipv6Header* GetIpv6Header (void);
  /**
   * \return Udp header if exists
   */
  virtual const UdpHeader* GetUdpHeader (void);
  /**
   * \return Tcp header if exists
   */
  virtual const TcpHeader* GetTcpHeader (void);
  
protected:
  CommonPacketData (const Ptr<const Packet> &packet);
  virtual bool HeaderFound (uint8_t type) = 0;
  virtual Header* GetHeader (Header* header);
  
private:
  Ptr<Packet> m_packet;
}

class Ipv4PacketData: public CommonPacketData
{
public:
  Ipv4PacketData (const Ptr<const Packet> &packet, const Ipv4Header &header);  
  virtual const Ipv4Header* GetIpv4Header (void);

protected:
  virtual bool HeaderFound (uint8_t type);

private:
  Ipv4Header* m_ipv4Header;
}

} // namespace mpls
} // namespace ns3

#endif /* MPLS_PROTOCOL_INFORMATION_H */
