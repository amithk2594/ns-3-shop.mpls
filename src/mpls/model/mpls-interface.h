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

#include "mpls-label-space.h"
#include "mpls.h"

namespace ns3 {

class Mpls;

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
  
  Interface ();
  virtual ~Interface ();
  
  void SetMpls (const Ptr<Mpls> &mpls); 
  void SetDevice (const Ptr<NetDevice> &device);
  void SetIfIndex (uint32_t index);

  /**
   * @brief Set link layer address resolving mode
   */
  void SetAddressResolvingMode (AddressResolvingMode mode);
  /**
   * @return address resolving mode
   */
  AddressResolvingMode GetAddressResolvingMode () const;
  /**
   * @return the underlying NetDevice.
   */
  Ptr<NetDevice> GetDevice () const;
  /**
   * @return Mpls 
   */
  Ptr<Mpls> GetMpls (void) const;
  /**
   * @return Interface index
   */
  uint32_t GetIfIndex (void) const;
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
  void Send (const Ptr<Packet>& packet, const Mac48Address &dest);
  /**
   * @brief Lookup Ipv4 interface
   */
  int32_t LookupIpv4Interface (void);
  /**
   * @brief Do lookup in the address resolving table against an ip address
   * @param dest The destination address to lookup the MAC address
   * @param hwaddr hardware address 
   * @return true if address found
   */
  bool LookupAddress (const Address &dest, Mac48Address& hwaddr);
  /**
   * @brief Add an address to the address resolving table
   */
  void AddAddress (const Address &dest, const Mac48Address &mac);
  /**
   * @brief Remove an address from the address resolving table
   */
  void RemoveAddress (const Address &dest);
  /**
   * @brief Remove all addresses
   */
  void RemoveAllAddresses (void); 
  /**
   * @brief Returns pointer to label space
   */
  LabelSpace* GetLabelSpace (void);
  /**
   * @brief Set minimum label value
   */
  void SetMinLabelValue (uint32_t value);
  /**
   * @brief Set maximum label value
   */
  void SetMaxLabelValue (uint32_t value);
protected:
  virtual void DoDispose (void);

private:
  Ptr<Mpls> m_mpls;
  Ptr<NetDevice> m_device;
  int32_t m_ipv4if;
  bool m_ifup;
  uint32_t m_ifIndex;
  AddressResolvingMode m_addressResolvingMode;
  LabelSpace m_labelSpace;
  
  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash> Ipv4Table;
  typedef sgi::hash_map<Ipv4Address, Mac48Address, Ipv4AddressHash>::iterator Ipv4TableIterator;

  Ipv4Table m_ipv4resolving;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
