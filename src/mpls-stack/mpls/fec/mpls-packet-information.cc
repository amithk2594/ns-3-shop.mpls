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
#include "mpls-packet-information.h"

namespace ns3 {
namespace mpls {

PacketContext::PacketContext (const Ptr<const Packet> &packet, const Ipv4Header &header)
  : m_packet (packet->Copy ()),
    m_ipv4Header (new Ipv4Header (header)), // XXX: will work?
    m_ipv6Header (NULL),
    m_udpHeader (NULL),
    m_udpSearched (false),
    m_tcpHeader (NULL),
    m_tcpSearched (false)
{
}

PacketContext::PacketContext (const Ptr<const Packet> &packet, const Ipv6Header &header)
  : m_packet (packet->Copy ()),
    m_ipv4Header (NULL), 
    m_ipv6Header (new Ipv6Header (header)),// XXX: will work?
    m_udpHeader (NULL),
    m_udpSearched (false),
    m_tcpHeader (NULL),
    m_tcpSearched (false)
{
}

PacketContext::~PacketContext ()
{
  delete m_ipv4Header;
  delete m_ipv6Header;
  delete m_tcpHeader;
  delete m_udpHeader;
}

const Ipv4Header*
PacketContext::GetIpv4 ()
{
  return m_ipv4Header;
}

const Ipv6Header*
PacketContext::GetIpv6 ()
{
  return m_ipv6Header;
}

const TcpHeader*
PacketContext::GetTcp ()
{
  if (!m_tcpSearched) 
    {
      // search tcp header
      m_tcpSearched = true;
    }
  return m_tcpHeader;
}

const UdpHeader*
PacketContext::GetUdp ()
{
  if (!m_udpSearched) 
    {
      // search tcp header
      m_udpSearched = true;
    }
  return m_udpHeader;
}

} // namespace mpls
} // namespace ns3
