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

#ifndef MPLS_INTERFACE_H
#define MPLS_INTERFACE_H

#include <ostream>
#include <stdint.h>

#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/arp-cache.h"
#include "ns3/ipv4-address.h"
#include "ns3/mac48-address.h"
#include "ns3/ptr.h"
#include "ns3/sgi-hashmap.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief
 * Mpls interface
 */
class Interface : public Object
{
public:
  static TypeId GetTypeId (void);
  
  /**
   * @enum AddressResolvingMode 
   * @brief Link layer address resolving mode
   */
  enum AddressResolvingMode 
    {
      AUTO = 0,
      DYNAMIC,
      STATIC
    };
  
  Interface (int32_t ifIndex);
  virtual ~Interface ();
  
  void SetNode (const Ptr<Node> &node); 
  void SetDevice (const Ptr<NetDevice> &device);

  /**
   * @brief Set link layer address resolving mode
   */
  void SetAddressResolvingMode (AddressResolvingMode mode);
  /**
   * @return address resolving mode
   */
  AddressResolvingMode SetAddressResolvingMode () const;
  /**
   * @return the underlying NetDevice.
   */
  Ptr<NetDevice> GetDevice () const;
  /**
   * @brief Returns true if this interface is enabled  
   */
  bool IsUp () const;
  /**
   * @brief Returns true if this interface is disabled
   */
  bool IsDown () const;
  /**
   * @brief Enable interface
   */
  void SetUp ();
  /**
   * @brief Disable interface
   */
  void SetDown ();
  /**
   * @brier Send packet 
   */
  bool Send (const Ptr<Packet>& packet);
  /**
   * @brief Returns interface index
   */
  int32_t GetIfIndex (void);  
  /**
   * @brief Do lookup in the address resolving table against an ip address
   * @param destination The destination IPv4 address to lookup the MAC address
   */
  const Mac48Address* LookupAddress (Ipv4Address &destination);
  /**
   * @brief Add an Ipv4Address to the address resolving table
   */
  void AddAddress (const Ipv4Address &dest, const Mac48Address &mac);
  /**
   * @brief Remove an Ipv4Address from the address resolving table
   */
  void RemoveAddress (const Ipv4Address &dest);

protected:
  virtual void DoDispose (void);

private:
  void DoSetup (void);

  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
  bool m_ifup;
  int32_t m_ifIndex;
  AddressResolvingMode m_addressResolvingMode;

  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash> Ipv4Table;
  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash>::iterator Ipv4TableIterator;

  Ipv4Table m_ipv4AddressResolvingTable;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
