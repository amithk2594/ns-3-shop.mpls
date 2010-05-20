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

#include "forwarding-equivalence-class.h"

namespace ns3 {
namespace mpls {

ForwardingEquivalenceClass::ForwardingEquivalenceClass ()
{
}

ForwardingEquivalenceClass::~ForwardingEquivalenceClass ()
{
}

uint32_t
ForwardingEquivalenceClass::GetMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  return -1;
}

uint32_t
ForwardingEquivalenceClass::GetMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  return -1;
}

} // namespace mpls
} // namespace ns3
