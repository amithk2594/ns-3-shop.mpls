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

#ifndef MPLS_ADDRESS_RESOLVING_TABLE_H
#define MPLS_ADDRESS_RESOLVING_TABLE_H

#include <stdint.h>
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include "ns3/mac48-address.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/sgi-hashmap.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief An Link layer address resolving table
 *
 * A table for translating layer 3 addresses to layer 2.
 */
class AddressResolvingTable : public Object
{
public:
  static TypeId GetTypeId (void);

  AddressResolvingTable ();
  ~AddressResolvingTable ();
  /**
   * @brief Do lookup in the table against an ip address
   * @param destination The destination IPv4 address to lookup the MAC address
   */
  const Mac48Address* Lookup (Ipv4Address &destination);
  /**
   * @brief Add an Ipv4Address to the table
   */
  void Add (const Ipv4Address &dest, const Mac48Address &mac);
  /**
   * @brief Remove an Ipv4Address from the table
   */
  void Remove (const Ipv4Address &dest);
  /**
   * @brief Clear the table of all entries
   */
  void Flush (void);

private:
  virtual void DoDispose (void);

  AddressResolvingTable (AddressResolvingTable const &);
  AddressResolvingTable& operator= (AddressResolvingTable const &);

  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash> Ipv4Table;
  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash>::iterator Ipv4TableIterator;
};


} // namespace mpls
} // namespace ns3

#endif /* MPLS_ADDRESS_RESOLVING_TABLE_H */
