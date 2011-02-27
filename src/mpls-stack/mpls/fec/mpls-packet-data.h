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

#ifndef MPLS_PACKET_DATA_H
#define MPLS_PACKET_DATA_H

#include <ostream>

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"

namespace ns3 {
namespace mpls {

class HeaderHolderBase
{
public:
  HeaderHolderBase ();
  virtual ~HeaderHolderBase ();
  virtual Header* Get () const;
  virtual void UnSet (void);
  virtual void Reset (void);
  virtual bool IsReset (void) const;

protected:
  enum {
    RESET = 0,
    SET,    
    UNSET
  } State;

  State m_state;

private:
  Header* m_header;
};

class FlyweightHeaderHolder: public HeaderHolderBase
{
public:
  FlyweightHeaderHolder ();
  virtual ~FlyweightHeaderHolder ();
  virtual void Set (Header* header);
}

class HeaderHolder: public HeaderHolderBase
{
public:
  HeaderHolder (Header* header);
  ~HeaderHolder ();
  Header* SetFromPacket (const Ptr<Packet>& packet);
};

/**
 * \ingroup Mpls
 * \brief
 * Packet context which holds common headers for Ipv4 and Ipv6
 */
class PacketData
{
public:
  PacketData ();
  ~PacketData ();
  
  /**
   * @brief Assign new packet and Ipv4 header.
   */
  void Assign (const Ptr<const Packet> &packet, const Ipv4Header &header);
  /**
   * \return Ipv4 header if exists
   */
  const Ipv4Header* GetIpv4Header (void);
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
  FlyweightHeaderHolder m_ipv4Holder;
  HeaderHolder m_tcpHolder;
  HeaderHolder m_udpHolder;
}

} // namespace mpls
} // namespace ns3

#endif /* MPLS_PACKET_DATA_H */
