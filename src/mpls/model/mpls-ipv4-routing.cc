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

#include "ipv4-to-mpls-routing.h"

NS_LOG_COMPONENT_DEFINE ("MplsIpv4Routing");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (MplsIpv4Routing);

MplsIpv4Routing::MplsIpv4Routing ()
  : m_ipv4 (0),
    m_mpls (0)
{
  NS_LOG_FUNCTION (this);
}

MplsIpv4Routing::~MplsIpv4Routing ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId
MplsIpv4Routing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsIpv4Routing")
    .SetParent<Ipv4RoutingProtocol> ()
    .AddConstructor<MplsIpv4Routing> ()
  ;
  return tid;
}

void
MplsIpv4Routing::DoDispose (void)
{
  m_ipv4 = 0;
  m_mpls = 0;
  m_routing = 0;
  Ipv4RoutingProtocol::DoDispose ();
}

Ptr<Ipv4Route>
MplsIpv4Routing::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << &oif << p << header);
  
  NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");

  return m_routing->RouteOutput (p, header, oif, sockerr);
}

bool
MplsIpv4Routing::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                       UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                       LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_ASSERT_MSG (m_mpls, "Mpls routing is not specified");

  MplsRouting::ForwardingStatus status = m_mpls->RouteInput (p, header, idev);

  switch (status)
  {
    case Mpls::IP_FORWARD:
      NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");
      return m_routing->RouteInput (p, header, idev, ucb, mcb, lcb, ecb);

    case Mpls::DROP_PACKET:
      ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      break;

    case Mpls::MPLS_FORWARDED:
      break;
  }

  return true;
}

void
MplsIpv4Routing::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_ASSERT_MSG (m_ipv4 == 0, "Ipv4 routing is specified already");
  NS_ASSERT (ipv4 != 0);
  m_ipv4 = ipv4;
}

void
MplsIpv4Routing::SetMpls (Ptr<MplsRouting> mpls)
{
  NS_ASSERT_MSG (m_mpls == 0, "Mpls routing already specified");
  NS_ASSERT (mpls != 0);
  m_mpls = mpls;
}

void
MplsIpv4Routing::NotifyInterfaceUp (uint32_t interface)
{
  NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");
  m_routing->NotifyInterfaceUp (interface);
}

void
MplsIpv4Routing::NotifyInterfaceDown (uint32_t interface)
{
  NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");
  m_routing->NotifyInterfaceDown (interface);
}

void
MplsIpv4Routing::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");
  m_routing->NotifyAddAddress (interface, address);
}

void
MplsIpv4Routing::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routing != 0, "Ipv4 routing is not specified");
  m_routing->NotifyRemoveAddress (interface, address);
}

void
MplsIpv4Routing::SetRouting (const Ptr<Ipv4RoutingProtocol> &routing)
{
  m_routing = routing;
  m_routing->SetIpv4 (m_ipv4);
}

Ptr<Ipv4RoutingProtocol>
MplsIpv4Routing::GetRouting (void) const
{
  return m_routing;
}

} // namespace mpls
} // namespace ns3
