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

#ifndef IPV4_TO_MPLS_ROUTING_H
#define IPV4_TO_MPLS_ROUTING_H

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4-interface-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/socket.h"

#include "mpls.h"

namespace ns3 {
namespace mpls {

class Ipv4ToMplsRouting : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);

  /**
   * \brief Consrtuctor
   */
  Ipv4ToMplsRouting ();
  /**
   * \brief Destructor
   */
  virtual ~Ipv4ToMplsRouting ();

  void SetIpv4 (const Ptr<Ipv4> &ipv4);
  void SetMpls (const Ptr<Mpls> &mpls);

  /**
   * \brief Set underlying Ipv4 routing protocol
   * \param routing
   */
  void SetRoutingProtocol (const Ptr<Ipv4RoutingProtocol> &routing);
  /**
   * \brief Get underlying Ipv4 routing protocol
   */
  Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (void) const;

  // Functions defined in base class Ipv4RoutingProtocol
  virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header,
                                      Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
  virtual bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);

protected:
  virtual void DoDispose (void);

private:
  Ptr<Ipv4> m_ipv4;
  Ptr<MplsRoutingProtocol> m_mpls;
  Ptr<Ipv4RoutingProtocol> m_routing;
};

} // namespace mpls
} // namespace ns3

#endif /* IPV4_TO_MPLS_ROUTING_H */
