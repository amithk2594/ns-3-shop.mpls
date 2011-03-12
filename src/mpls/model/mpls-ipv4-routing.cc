/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/assert.h"

#include "mpls-ipv4-routing.h"

NS_LOG_COMPONENT_DEFINE ("mpls::Ipv4Routing");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (Ipv4Routing);

Ipv4Routing::Ipv4Routing ()
  : m_mpls (0),
    m_ipv4 (0)
{
  NS_LOG_FUNCTION (this);
}

Ipv4Routing::~Ipv4Routing ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId
Ipv4Routing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Ipv4Routing")
    .SetParent<Ipv4RoutingProtocol> ()
    .AddConstructor<Ipv4Routing> ()
  ;
  return tid;
}

void
Ipv4Routing::DoDispose (void)
{
  m_mpls = 0;
  m_routingProtocol = 0;
  Ipv4RoutingProtocol::DoDispose ();
}

Ptr<Ipv4Route>
Ipv4Routing::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << &oif << p << header);
  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object to process packet");
  return m_routingProtocol->RouteOutput (p, header, oif, sockerr);
}

bool
Ipv4Routing::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                                       UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                       LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_ASSERT_MSG (m_mpls != 0, "Mpls protocol should be specified");

  // check if can process ipv4 packet
  Ptr<Interface> iface = m_mpls->GetInterfaceForDevice (idev);

  // use underlying ipv4 routing if mpls interface is enabled for the device
  if (iface != 0 && iface->IsUp ())
    {
      return m_routingProtocol->RouteInput (p, header, idev, ucb, mcb, lcb, ecb);
    }

  Ptr<Packet> packet = p->Copy ();
  
  m_demux.Assign (p, header);
  
  Ptr<FecToNhlfe> ftn = m_mpls->LookupFtn (m_demux);
  
  if (ftn == 0)
    {
      NS_LOG_LOGIC ("Dropping received packet -- FTN not found");
      //m_mpls->m_dropTrace (...)
      ecb (packet, header, Socket::ERROR_NOROUTETOHOST);
      return false;
    }

  LabelStack stack;
  m_mpls->MplsForward (packet, ftn, stack, header.GetTtl ());

  return true;
}

void
Ipv4Routing::SetMpls (Ptr<MplsProtocol> mpls)
{
  NS_ASSERT_MSG (m_mpls == 0, "Mpls protocol object already set");
  NS_ASSERT (mpls != 0);
  m_mpls = mpls;
}

void
Ipv4Routing::SetIpv4 (Ptr<Ipv4> ipv4)
{
  m_ipv4 = ipv4;
}

void
Ipv4Routing::NotifyInterfaceUp (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);
  
  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object");
  m_routingProtocol->NotifyInterfaceUp (interface);
}

void
Ipv4Routing::NotifyInterfaceDown (uint32_t interface)
{
  NS_LOG_FUNCTION (this << interface);

  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object");
  m_routingProtocol->NotifyInterfaceDown (interface);
}

void
Ipv4Routing::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  
  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object");
  m_routingProtocol->NotifyAddAddress (interface, address);
}

void
Ipv4Routing::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface << address);
  
  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object");
  m_routingProtocol->NotifyRemoveAddress (interface, address);
}

void
Ipv4Routing::SetRoutingProtocol (const Ptr<Ipv4RoutingProtocol> &routingProtocol)
{
  m_routingProtocol = routingProtocol;
  routingProtocol->SetIpv4 (m_ipv4);
}

Ptr<Ipv4RoutingProtocol>
Ipv4Routing::GetRoutingProtocol (void) const
{
  return m_routingProtocol;
}

void
Ipv4Routing::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Need Ipv4 routing object");
  m_routingProtocol->PrintRoutingTable (stream);
}

} // namespace mpls
} // namespace ns3
