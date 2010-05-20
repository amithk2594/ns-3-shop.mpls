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

#ifndef IP_TO_MPLS_ROUTING_HELPER_H
#define IP_TO_MPLS_ROUTING_HELPER_H

#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/ipv4-routing-helper.h"
#include "ns3/ipv6-routing-helper.h"

namespace ns3 {

class Ipv4ToMplsRoutingHelper : public Ipv4RoutingHelper
{
public:
  Ipv4ToMplsRoutingHelper ();
  Ipv4ToMplsRoutingHelper (const Ipv4ToMplsRoutingHelper &o);
  Ipv4ToMplsRoutingHelper* Copy (void) const;
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;
};

class Ipv6ToMplsRoutingHelper : public Ipv6RoutingHelper
{
public:
  Ipv6ToMplsRoutingHelper ();
  Ipv6ToMplsRoutingHelper (const Ipv6ToMplsRoutingHelper &);
  Ipv6ToMplsRoutingHelper* Copy (void) const;
  virtual Ptr<Ipv6RoutingProtocol> Create (Ptr<Node> node) const;
};

} // namespace ns3

#endif /* IP_TO_MPLS_ROUTING_HELPER_H */
