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

#ifndef MPLS_IPV4_PROTOCOL_H
#define MPLS_IPV4_PROTOCOL_H

#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-routing-protocol.h"

#include "mpls.h"
#include "mpls-ipv4-routing.h"

namespace ns3 {

class Mpls;

namespace mpls {

class Ipv4Routing;

/**
 * \brief Ipv4 layer for Mpls
 */
class Ipv4Protocol : public Ipv4L3Protocol
{
public:
  static TypeId GetTypeId (void);

  Ipv4Protocol ();
  virtual ~Ipv4Protocol ();

  // methods defined in parent class
  void SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol);
  Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (void) const;

protected:
  void NotifyNewAggregate ();

private:
  Ptr<Ipv4Routing> m_routingProtocol;
  Ptr<Mpls> m_mpls;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_IPV4_PROTOCOL_H */
