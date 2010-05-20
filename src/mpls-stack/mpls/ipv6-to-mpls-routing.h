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

#ifndef IPV6_TO_MPLS_ROUTING_H
#define IPV6_TO_MPLS_ROUTING_H

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/ipv6.h"
#include "ns3/ipv6-address.h"
#include "ns3/ipv6-interface-address.h"
#include "ns3/ipv6-header.h"
#include "ns3/ipv6-route.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/socket.h"

#include "mpls-routing-protocol.h"

namespace ns3 {
namespace mpls {

class Ipv6ToMplsRouting : public Ipv6RoutingProtocol
{
public:
  static TypeId GetTypeId (void);

  Ipv6ToMplsRouting ();
  virtual ~Ipv6ToMplsRouting ();

  virtual Ptr<Ipv6Route> RouteOutput (Ptr<Packet> p, const Ipv6Header &header,
                                      Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);

  virtual bool RouteInput (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);

  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address);
  virtual void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop,
                               uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  virtual void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop,
                                  uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());

  virtual void SetIpv6 (Ptr<Ipv6> ipv6);
  virtual void SetMpls (Ptr<MplsRoutingProtocol> mpls);

  // set underlying ipv6 routing protocol
  void SetRoutingProtocol (Ptr<Ipv6RoutingProtocol> routingProtocol);
  // get underlying ipv6 routing protocol
  Ptr<Ipv6RoutingProtocol> GetRoutingProtocol (void) const;

protected:
  virtual void DoDispose (void);

private:
  Ptr<Ipv6> m_ipv6;
  Ptr<MplsRoutingProtocol> m_mpls;
  Ptr<Ipv6RoutingProtocol> m_routingProtocol;
};

} // namespace mpls
} // Namespace ns3

#endif /* IPV6_TO_MPLS_ROUTING_H */
