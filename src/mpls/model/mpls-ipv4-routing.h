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

#ifndef MPLS_IPV4_ROUTING_H
#define MPLS_IPV4_ROUTING_H

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

#include "mpls-packet-demux.h"
#include "mpls-protocol.h"

namespace ns3 {
namespace mpls {

class PacketDemux;
class MplsProtocol;

/**
 * \brief
 * Mpls ipv4 routing hook
 */
class Ipv4Routing : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);
  /**
   * @brief Constructor
   */
  Ipv4Routing ();
  /**
   * @brief Destructor
   */
  virtual ~Ipv4Routing ();
  /**
   * @brief Set underlying Ipv4 routing protocol
   */
  void SetRoutingProtocol (const Ptr<Ipv4RoutingProtocol> &routingProtocol);
  /**
   * @brief Get underlying Ipv4 routing protocol
   */
  Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (void) const;
  
  // Functions defined in base class Ipv4RoutingProtocol
  Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
  bool RouteInput  (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, 
                             UnicastForwardCallback ucb, MulticastForwardCallback mcb, 
                             LocalDeliverCallback lcb, ErrorCallback ecb);
  void NotifyInterfaceUp (uint32_t interface);
  void NotifyInterfaceDown (uint32_t interface);
  void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  void PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const;

  void SetMpls (Ptr<MplsProtocol> mpls);
  void SetIpv4 (Ptr<Ipv4> ipv4);
    
protected:
  void DoDispose (void);

private:
  Ptr<MplsProtocol> m_mpls;
  Ptr<Ipv4> m_ipv4;  
  Ptr<Ipv4RoutingProtocol> m_routingProtocol;
  PacketDemux m_demux;  
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_IPV4_ROUTING_H */
