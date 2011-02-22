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
#include "mpls-packet-data.h"

namespace ns3 {
namespace mpls {

CommonPacketData::CommonPacketData (const Ptr<const Packet> &packet)
  : m_packet (packet->Copy ()),
    m_udpHeader (UNRESOLVED),
    m_tcpHeader (UNRESOLVED)
{
}

CommonPacketData::~CommonPacketData ()
{
  delete m_tcpHeader;
  delete m_udpHeader;
}

const Ipv4Header*
CommonPacketData::GetIpv4Header ()
{
  return 0;
}

const Ipv6Header*
CommonPacketData::GetIpv6Header ()
{
  return 0;
}

const TcpHeader*
CommonPacketData::GetTcp ()
{
  if (m_tcpHeader == UNRESOLVED)
    {
      m_tcpHeader = HeaderFound (6) ? GetHeader (new TcpHeader ()): 0;
    }
  return m_tcpHeader;
}

const UdpHeader*
CommonPacketData::GetUdp ()
{
  if (m_udpHeader == UNRESOLVED) 
    {
      m_udpHeader = HeaderFound (17) ? GetHeader (new UdpHeader ()) : 0;
    }
  return m_udpHeader;
}

Header*
CommonPacketData::GetHeader (Header* header)
{
  m_packet->RemoveHeader (*header);
  return header;
}

Ipv4PacketData::Ipv4PacketData (const Ptr<const Packet> &packet, const Ipv4Header &header)
  : CommonPacketData (packet),
    m_ipv4Header (header)
{
}

const Ipv4Header*
Ipv4PacketData::GetIpv4Header ()
{
  return &m_ipv4Header;
}
  
bool
Ipv4PacketData::HeaderFound (unit8_t type)
{
  return m_ipv4Header.GetProtocol() == type;
}

} // namespace mpls
} // namespace ns3
