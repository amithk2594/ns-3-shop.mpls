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

#ifndef IPV4_ADDRESS_PREFIX_H
#define IPV4_ADDRESS_PREFIX_H

#include <ostream>

#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"

#include "forwarding-equivalence-class.h"

namespace ns3 {
namespace mpls {

class Ipv4AddressPrefix : public ForwardingEquivalenceClass
{
public:
  static const uint32_t FEC_MATCH_VALUE = 0x200;

  Ipv4AddressPrefix ();
  Ipv4AddressPrefix (const Ipv4AddressPrefix &fec);
  Ipv4AddressPrefix (const Ipv4Address &address);
  Ipv4AddressPrefix (const Ipv4Address &address, uint8_t prefix);
  Ipv4AddressPrefix (char const *address);

  virtual ~Ipv4AddressPrefix ();

  void Set (const Ipv4Address &address);
  void Set (const Ipv4Address &address, uint8_t prefix);
  void Set (char const *address);

  Ipv4Address GetAddress (void) const;
  Ipv4Mask GetMask (void) const;

  virtual uint32_t GetMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual void Print (std::ostream &os) const;
  virtual Ipv4AddressPrefix* Copy (void) const;

private:
  Ipv4Address m_address;
  uint8_t m_prefix;
};

std::ostream& operator<< (std::ostream& os, const Ipv4AddressPrefix &fec);

} // namespace mpls
} // namespace ns3

#endif /* IPV4_ADDRESS_PREFIX_H */
