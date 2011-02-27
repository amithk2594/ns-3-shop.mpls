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

HeaderHolderBase::HeaderHolderBase ()
  : m_state (CLEAR),
    m_header (0)
{
}

HeaderHolderBase::~HeaderHolderBase ()
{
}

Header*
HeaderHolderBase::Get (void)
{
  return m_state == SET ? m_header : NULL;
}

bool
HeaderHolderBase::UnSet (void) const
{
  m_state = UNSET;
}

void
HeaderHolderBase::Reset (void)
{
  m_state = RESET;
}

bool
HeaderHolderBase::IsReset (void) const
{
  return m_state == RESET;
}

FlyweightHeaderHolder::~UnmanagedHeaderHolder ()
{
}

void
FlyweightHeaderHolder::Set (Header* header)
{
  m_header = header;
  m_state = SET;
}

HeaderHolder::HeaderHolder (Header* header)
  : m_header (header)
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

PacketData::PacketData (const Ptr<Node>& node)
  : m_node (node),
    m_packet (0),
    m_ipv4Holder (),
    m_tcpHolder (new TcpHeader),
    m_udpHolder (new UdpHeader),
{
}

PacketData::~PacketData ()
{
}

void
PacketData::Assign (const Ptr<const Packet> &packet, const Ipv4Header &header)
{
  ResetHolders ();
  m_ipv4Holder.Set (&header);
  m_packet = packet->Copy ();
}

void
PacketData::ResetHeaders (void)
{
  m_ipv4Holder.Reset ();
  m_tcpHolder.Reset ();
  m_udpHolder.Reset ();
}

Header*
PacketData::GetHeaderFromHolder (uint8_t protocol, HeaderHolder& holder)
{
  if (holder.IsReset ()) 
    {
      Ipv4Header* ipv4 = GetIpv4Header ();
      if (ipv4 && ipv4->GetProtocol() == procotol)
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
PacketData::GetIpv4Header ()
{
  return (Ipv4Header*)m_ipv4Holder.Get ();
}

const TcpHeader*
PacketData::GetTcp ()
{
  return (TcpHeader*)GetHeaderFromHolder (6, m_tcpHolder);
}

const UdpHeader*
PacketData::GetUdp ()
{
  return (UdpHeader*)GetHeaderFromHolder (17, m_udpHolder);
}

} // namespace mpls
} // namespace ns3
