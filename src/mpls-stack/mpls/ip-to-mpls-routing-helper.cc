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

#include "ip-to-mpls-routing-helper.h"
#include "mpls-routing-protocol.h"
#include "ipv4-to-mpls-routing.h"
#include "ipv6-to-mpls-routing.h"

NS_LOG_COMPONENT_DEFINE ("IpToMplsRoutingHelper");

namespace ns3 {

using namespace mpls;

Ipv4ToMplsRoutingHelper::Ipv4ToMplsRoutingHelper ()
{
}

Ipv4ToMplsRoutingHelper::Ipv4ToMplsRoutingHelper (const Ipv4ToMplsRoutingHelper &o)
{
}

Ipv4ToMplsRoutingHelper*
Ipv4ToMplsRoutingHelper::Copy (void) const
{
  return new Ipv4ToMplsRoutingHelper (*this);
}

Ptr<Ipv4RoutingProtocol>
Ipv4ToMplsRoutingHelper::Create (Ptr<Node> node) const
{
  Ptr<MplsRoutingProtocol> mpls = node->GetObject<MplsRoutingProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "Ipv4ToRoutingHelper::Create (): an MplsRoutingProtocol object not exists");
  Ptr<Ipv4ToMplsRouting> routing = CreateObject<Ipv4ToMplsRouting> ();
  routing->SetMpls (mpls);
  return routing;
}

Ipv6ToMplsRoutingHelper::Ipv6ToMplsRoutingHelper ()
{
}

Ipv6ToMplsRoutingHelper::Ipv6ToMplsRoutingHelper (const Ipv6ToMplsRoutingHelper &o)
{
}

Ipv6ToMplsRoutingHelper*
Ipv6ToMplsRoutingHelper::Copy (void) const
{
  return new Ipv6ToMplsRoutingHelper (*this);
}

Ptr<Ipv6RoutingProtocol>
Ipv6ToMplsRoutingHelper::Create (Ptr<Node> node) const
{
  Ptr<MplsRoutingProtocol> mpls = node->GetObject<MplsRoutingProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "Ipv6ToRoutingHelper::Create (): an MplsRoutingProtocol object not exists");
  Ptr<Ipv6ToMplsRouting> routing = CreateObject<Ipv6ToMplsRouting> ();
  routing->SetMpls (mpls);
  return routing;
}

} // namespace ns3
