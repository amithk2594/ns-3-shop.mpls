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

NS_LOG_COMPONENT_DEFINE ("Ipv4ToMplsRouting");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (Ipv4ToMplsRouting);

Ipv4ToMplsRouting::Ipv4ToMplsRouting ()
  : m_ipv4 (0),
    m_mpls (0)
{
  NS_LOG_FUNCTION (this);
}

Ipv4ToMplsRouting::~Ipv4ToMplsRouting ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId
Ipv4ToMplsRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Ipv4ToMplsRouting")
    .SetParent<Ipv4RoutingProtocol> ()
    .AddConstructor<Ipv4ToMplsRouting> ()
  ;
  return tid;
}

void
Ipv4ToMplsRouting::DoDispose (void)
{
  m_ipv4 = 0;
  m_mpls = 0;
  m_routingProtocol = 0;
  Ipv4RoutingProtocol::DoDispose ();
}

Ptr<Ipv4Route>
Ipv4ToMplsRouting::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << &oif << p << header);

  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::RouteOutput ():"
                 "Need a Ipv4 routing protocol to process packets");

  return m_routingProtocol->RouteOutput (p, header, oif, sockerr);
}

bool
Ipv4ToMplsRouting::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                       UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                       LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << &idev << p << header);
  NS_ASSERT_MSG (m_mpls, "Ipv4ToMplsRouting::RouteInput ():"
                 "Need a MplsRoutingProtocol object to process packets");

  MplsRoutingProtocol::MplsRoutingErrno result = m_mpls->RouteInput (p, header, idev);

  switch (result)
    {
    case MplsRoutingProtocol::ERROR_IPROUTING:
      NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::RouteInput ():"
                     "Need a Ipv4 routing protocol to process packets");
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
Ipv4ToMplsRouting::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this << ipv4);
  NS_ASSERT_MSG (m_ipv4 == 0, "Ipv4ToMplsRouting::SetIpv4 (): m_ipv4 != 0");
  NS_ASSERT (ipv4 != 0);
  m_ipv4 = ipv4;
}

void
Ipv4ToMplsRouting::SetMpls (Ptr<MplsRoutingProtocol> mpls)
{
  NS_LOG_FUNCTION (this << mpls);
  NS_ASSERT_MSG (m_mpls == 0, "Ipv4ToMplsRouting::SetMpls (): m_mpls != 0");
  NS_ASSERT (mpls != 0);
  m_mpls = mpls;
}

void
Ipv4ToMplsRouting::NotifyInterfaceUp (uint32_t interface)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv4 routing protocol to process InterfaceUp notification");
  m_routingProtocol->NotifyInterfaceUp (interface);
}

void
Ipv4ToMplsRouting::NotifyInterfaceDown (uint32_t interface)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv4 routing protocol to process InterfaceDown notification");
  m_routingProtocol->NotifyInterfaceDown (interface);
}

void
Ipv4ToMplsRouting::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv4 routing protocol to process AddAddress notification");
  m_routingProtocol->NotifyAddAddress (interface, address);
}

void
Ipv4ToMplsRouting::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Ipv4ToMplsRouting::NotifyInterfaceUp (): "
                 "Need a Ipv4 routing protocol to process RemoveAddress notification");
  m_routingProtocol->NotifyRemoveAddress (interface, address);
}

void
Ipv4ToMplsRouting::SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol)
{
  NS_LOG_FUNCTION (this << routingProtocol);
  m_routingProtocol = routingProtocol;
  m_routingProtocol->SetIpv4 (m_ipv4);
}

Ptr<Ipv4RoutingProtocol>
Ipv4ToMplsRouting::GetRoutingProtocol (void) const
{
  return m_routingProtocol;
}

} // namespace mpls
} // namespace ns3
