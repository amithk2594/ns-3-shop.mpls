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

#include "ipv6-to-mpls-routing.h"

NS_LOG_COMPONENT_DEFINE ("Ipv6ToMplsRouting");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (Ipv6ToMplsRouting);

TypeId
Ipv6ToMplsRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Ipv6ToMplsRouting")
    .SetParent<Ipv6RoutingProtocol> ()
    .AddConstructor<Ipv6ToMplsRouting> ()
  ;
  return tid;
}

Ipv6ToMplsRouting::Ipv6ToMplsRouting ()
  : m_ipv6 (0),
    m_mpls (0)
{
  NS_LOG_FUNCTION (this);
}

Ipv6ToMplsRouting::~Ipv6ToMplsRouting ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Ipv6ToMplsRouting::DoDispose (void)
{
  m_ipv6 = 0;
  m_mpls = 0;
  m_routingProtocol = 0;
  Ipv6RoutingProtocol::DoDispose ();
}

Ptr<Ipv6Route>
Ipv6ToMplsRouting::RouteOutput (Ptr<Packet> p, const Ipv6Header &header, Ptr<NetDevice> oif,
                                        Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << &oif << p << header);

  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::RouteOutput ():"
                 "Need a Ipv6 routing protocol to process packets");

  return m_routingProtocol->RouteOutput (p, header, oif, sockerr);
}

bool
Ipv6ToMplsRouting::RouteInput (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev,
                                       UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << &idev << p << header);
  NS_ASSERT_MSG (m_mpls, "Ipv6ToMplsRouting::RouteInput (): Need a MplsRoutingProtocol object to process packets");

  MplsRoutingProtocol::MplsRoutingErrno result = m_mpls->RouteInput6 (p, header, idev);

  switch (result)
    {
    case MplsRoutingProtocol::ERROR_IPROUTING:
      NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::RouteInput ():"
                     "Need a Ipv6 routing protocol to process packets");
      return m_routingProtocol->RouteInput (p, header, idev, ucb, mcb, lcb, ecb);

    case MplsRoutingProtocol::ERROR_NOROUTE:
    case MplsRoutingProtocol::ERROR_NOTERROR:
      // silently drop package if not sended
      // ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      break;
    }

  return true;
}

void
Ipv6ToMplsRouting::SetIpv6 (Ptr<Ipv6> ipv6)
{
  NS_LOG_FUNCTION (this << ipv6);
  NS_ASSERT_MSG (m_ipv6 == 0, "Ipv6ToMplsRouting::SetIpv4 (): m_ipv6 != 0");
  NS_ASSERT (ipv6 != 0);
  m_ipv6 = ipv6;
}

void
Ipv6ToMplsRouting::SetMpls (Ptr<MplsRoutingProtocol> mpls)
{
  NS_LOG_FUNCTION (this << mpls);
  NS_ASSERT_MSG (m_mpls == 0, "Ipv6ToMplsRouting::SetMpls (): m_mpls != 0");
  NS_ASSERT (mpls != 0);
  m_mpls = mpls;
}

void
Ipv6ToMplsRouting::NotifyInterfaceUp (uint32_t interface)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process InterfaceUp notification");
  m_routingProtocol->NotifyInterfaceUp (interface);
}

void
Ipv6ToMplsRouting::NotifyInterfaceDown (uint32_t interface)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process InterfaceDown notification");
  m_routingProtocol->NotifyInterfaceDown (interface);
}

void
Ipv6ToMplsRouting::NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process AddAddress notification");
  m_routingProtocol->NotifyAddAddress (interface, address);
}

void
Ipv6ToMplsRouting::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process RemoveAddress notification");
  m_routingProtocol->NotifyRemoveAddress (interface, address);
}

void
Ipv6ToMplsRouting::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop,
                                           uint32_t interface, Ipv6Address prefixToUse)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process AddRoute notification");
  m_routingProtocol->NotifyAddRoute (dst, mask, nextHop, interface, prefixToUse);
}

void
Ipv6ToMplsRouting::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop,
                                              uint32_t interface, Ipv6Address prefixToUse)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv6ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv6 routing protocol to process RemoveRoute notification");
  m_routingProtocol->NotifyAddRoute (dst, mask, nextHop, interface, prefixToUse);
}

void
Ipv6ToMplsRouting::SetRoutingProtocol (Ptr<Ipv6RoutingProtocol> routingProtocol)
{
  NS_LOG_FUNCTION (this << routingProtocol);
  m_routingProtocol = routingProtocol;
  m_routingProtocol->SetIpv6 (m_ipv6);
}

Ptr<Ipv6RoutingProtocol>
Ipv6ToMplsRouting::GetRoutingProtocol (void) const
{
  return m_routingProtocol;
}

} // namespace mpls
} // namespace ns3
