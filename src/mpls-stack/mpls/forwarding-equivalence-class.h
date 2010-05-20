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

#ifndef FORWARDING_EQUIVALENCE_CLASS_H
#define FORWARDING_EQUIVALENCE_CLASS_H

#include <ostream>
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"

namespace ns3 {
namespace mpls {

class ForwardingEquivalenceClass
{
public:
  ForwardingEquivalenceClass ();
  virtual ~ForwardingEquivalenceClass ();

  virtual uint32_t GetMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual uint32_t GetMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual ForwardingEquivalenceClass* Copy (void) const = 0;
  virtual void Print (std::ostream &os) const = 0;
};

} // namespace mpls
} // namespace ns3

#endif /* FORWARDING_EQUIVALENCE_CLASS_H */
