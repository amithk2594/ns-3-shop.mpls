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

#ifndef MPLS_STACK_HELPER_H
#define MPLS_STACK_HELPER_H

#include "ns3/node.h"
#include "ns3/ptr.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4-routing-helper.h"
#include "ns3/ipv6-routing-helper.h"
#include "ns3/internet-stack-helper.h"

#include "mpls-routing-protocol.h"

namespace ns3 {

using namespace mpls;

class MplsStackHelper
{
public:
  MplsStackHelper ();
  virtual ~MplsStackHelper ();

public:
  void SetIpv4Enabled (bool enabled);
  void SetIpv6Enabled (bool enabled);
  void SetRoutingHelper (const Ipv4RoutingHelper &routing);
  void SetRoutingHelper (const Ipv6RoutingHelper &routing);
  void Install (Ptr<Node> node) const;
  void Install (NodeContainer c) const;
  void Assign (const NetDeviceContainer &c) const;
  void Assign (Ptr<NetDevice> device) const;
private:
  void Initialize ();
  void SetIpv4AllRoutersMulticast (Ptr<Node> node, Ptr<MplsRoutingProtocol> mpls) const;
  Ptr<Ipv4ToMplsRouting> GetIpv4ToMplsRouting (Ptr<Node> node) const;
  Ptr<Ipv6ToMplsRouting> GetIpv6ToMplsRouting (Ptr<Node> node) const;
  Ptr<Ipv4StaticRouting> GetStaticRouting (Ptr<Node> node) const;

  InternetStackHelper m_internetStackHelper;
  const Ipv4RoutingHelper *m_routing;
  const Ipv6RoutingHelper *m_routingv6;
  bool m_ipv4Enabled;
  bool m_ipv6Enabled;
};

} // namespace ns3

#endif /* MPLS_STACK_HELPER_H */
