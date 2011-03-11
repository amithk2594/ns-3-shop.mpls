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

#include "ns3/log.h"
#include "ns3/assert.h"

#include "mpls-packet-demux.h"

namespace ns3 {
namespace mpls {

HeaderHolder::HeaderHolder (Header* header)
  : m_state (RESET),
    m_header (header)
{
}

HeaderHolder::~HeaderHolder ()
{
  delete m_header;
}

Header*
HeaderHolder::SetFromPacket (const Ptr<Packet>& packet)
{
  packet->RemoveHeader (*m_header);
  m_state = SET;
  return m_header;
}


Header*
HeaderHolder::Get (void) const
{
  return m_state == SET ? m_header : NULL;
}

void
HeaderHolder::UnSet (void)
{
  m_state = UNSET;
}

void
HeaderHolder::Reset (void)
{
  m_state = RESET;
}

bool
HeaderHolder::IsReset (void) const
{
  return m_state == RESET;
}

PacketDemux::PacketDemux ()
  : m_packet (0),
    m_ipv4Header (0),
    m_ipv6Header (0),
    m_tcpHolder (new TcpHeader),
    m_udpHolder (new UdpHeader)
{
}

PacketDemux::~PacketDemux ()
{
}

void
PacketDemux::Assign (const Ptr<const Packet> &packet, const Ipv4Header &header)
{
  ResetHolders ();
  m_ipv4Header = &header;
  m_packet = packet->Copy ();
}

void
PacketDemux::Assign (const Ptr<const Packet> &packet, const Ipv6Header &header)
{
  NS_ASSERT_MSG (0, "PacketDemux for Ipv6 not implemented");
}

void
PacketDemux::ResetHolders (void)
{
  m_ipv4Header = 0;
  m_ipv6Header = 0;
  m_tcpHolder.Reset ();
  m_udpHolder.Reset ();
}

Header*
PacketDemux::GetHeaderFromHolder (uint8_t protocol, HeaderHolder& holder)
{
  if (holder.IsReset ()) 
    {
      if (m_ipv4Header && m_ipv4Header->GetProtocol() == protocol)
        {
          return holder.SetFromPacket (m_packet);
        }

//      Ipv6Header* ipv6 = GetIpv6Header ();
//      XXX: should use Ipv6ExtensionDemux here
//      if (ipv6)
//        {
//           && ipv6->GetNextHeader() == procotol
//          holder.UnSet ();
//        }

      holder.UnSet ();
      return 0;
    }
  return holder.Get ();
}

const Ipv4Header*
PacketDemux::GetIpv4Header ()
{
  return m_ipv4Header;
}

const Ipv6Header*
PacketDemux::GetIpv6Header ()
{
  return 0;
}

const TcpHeader*
PacketDemux::GetTcpHeader ()
{
  return (TcpHeader*)GetHeaderFromHolder (6, m_tcpHolder);
}

const UdpHeader*
PacketDemux::GetUdpHeader ()
{
  return (UdpHeader*)GetHeaderFromHolder (17, m_udpHolder);
}

} // namespace mpls
} // namespace ns3
