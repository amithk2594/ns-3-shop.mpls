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

#ifndef MPLS_ROUTING_H
#define MPLS_ROUTING_H

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
#include "ns3/traced-callback.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * Unicast Multiprotocol Label Switching. RFC 3031, 3032.
 */
class MplsRouting : public Object
{
public:
  /**
   * @enum ForwardingType
   * @brief forwarding type
   */
  enum Status
  {
    MPLS_FORWARD,
    IP_FORWARD,
    DROP_PACKET
  };

  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  /**
   * @brief Constructor
   */
  MplsRouting ();
  /**
   * @brief Destructor
   */
  virtual ~MplsRouting ();
  /**
   * @brief Lower layer calls this method after calling L3Demux::Lookup
   * @param device network device
   * @param p the packet
   * @param protocol protocol value
   * @param from address of the correspondant
   * @param to address of the destination
   * @param packetType type of the packet
   */
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to,
    NetDevice::PacketType packetType);
  /**
   * @brief Route Ipv4 packet
   * @param packet Packet to be routed
   * @param header Ipv4 header
   * @param idev Input interface Netdevice.
   */
  Status RouteInput (const Ptr<const Packet> &packet, const Ipv4Header &header,
    const Ptr<const NetDevice> &idev) const;
  /**
   * @brief Assign a mpls interface to the device
   * @param device NetDevice
   * @return interface index
   */
  uint32_t AddInterface (const Ptr<NetDevice> &device);
  /**
   * \brief Get mpls interface by index
   * \param index interface index
   */
  Ptr<MplsInterface> GetInterface (uint32_t index) const;
  /**
   * \brief Get mpls interface for specified device
   * \return MplsInterface
   */
  Ptr<MplsInterface> GetInterfaceForDevice (const Ptr<NetDevice> &device) const;
  /**
   * \brief Get mpls interface count
   */
  uint32_t GetNInterfaces (void) const;
  /**
   * \brief Get global forwarding information base instance
   */
  const Ptr<MplsFib>& GetFib (void) const;

protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  void SetNode (Ptr<Node> node);

  void IpForward (Ptr<Packet> packet, Ptr<NetDevice> odev, uint8_t ttl) const;
  void MplsForward (Ptr<Packet> packet, const MplsLabelStack &stack, const Header* ipHeader, Ptr<NetDevice> odev) const;

private:
  typedef std::vector<Ptr<MplsInterface> > MplsInterfaceVector;

  Ptr<Node> m_node;
  MplsInterfaceVector m_interfaces;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_H */
