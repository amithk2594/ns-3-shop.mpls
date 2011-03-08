/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin, Stefano Avallone
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

#ifndef MPLS_PROTOCOL_H
#define MPLS_PROTOCOL_H

#include <list>
#include <ostream>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv6-header.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/ipv6-route.h"
#include "ns3/traced-callback.h"

#include "mpls-lib-entry.h"
#include "mpls-label-stack.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * Unicast Multiprotocol Label Switching. RFC 3031, 3032.
 */
class MplsProtocol : public Object
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  MplsProtocol ();
  virtual ~MplsProtocol ();

  void SetNode (const Ptr<Node>& node);
  
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                const Address &to, NetDevice::PacketType packetType);
  /**
   * @param device device to add to the list of Mpls interfaces
   * @return interface index of the Mpls interface added
   */
  uint32_t AddInterface (const Ptr<NetDevice> &device);
  /**
   * @brief Get Mpls interface by index
   * @param index Mpls interface index
   */
  Ptr<MplsInterface> GetInterface (uint32_t index) const;
  /**
   * @brief Get Mpls interface for specified device
   * @return Mpls interface
   */
  Ptr<MplsInterface> GetInterfaceForDevice (const Ptr<NetDevice> &device) const;
  /**
   * @brief The number of Mpls interfaces added
   */
  uint32_t GetNInterfaces (void) const;

protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  typedef std::vector<Ptr<MplsInterface> > MplsInterfaceList;
  
  Ptr<Node> m_node;
  MplsInterfaceList m_interfaces;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_PROTOCOL_H */
