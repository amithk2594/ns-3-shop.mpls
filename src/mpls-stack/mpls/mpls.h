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

#ifndef MPLS_H
#define MPLS_H

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

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * Unicast Multiprotocol Label Switching. RFC 3031, 3032.
 */
class Mpls : public Object
{
public:
  /**
   * \enum ForwardingType
   * \brief forwarding type
   */
  enum ForwardingType
  {
    MPLS_FORWARDING,
    IP_FORWARDING,
    NO_ROUTE
  };

  /**
   * \enum LabelSpace
   * \brief label space
   */
  enum LabelSpace
  {
    PER_INTERFACE_LABEL_SPACE = 0,
    PER_PLATFORM_LABEL_SPACE
  };

  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;
  /**
   * \brief Constructor
   */
  Mpls ();
  /**
   * \brief Destructor
   */
  virtual ~Mpls ();
  /**
   * \brief Lower layer calls this method after calling L3Demux::Lookup
   * \param device network device
   * \param p the packet
   * \param protocol protocol value
   * \param from address of the correspondant
   * \param to address of the destination
   * \param packetType type of the packet
   */
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to,
    NetDevice::PacketType packetType);
  /**
   * \brief Route Ipv4 packet
   * \param packet Packet to be routed
   * \param header Ipv4 header
   * \param idev Input interface Netdevice.
   */
  enum ForwardingType RouteInput (const Ptr<const Packet> &packet, const Ipv4Header &header,
    const Ptr<const NetDevice> &idev) const;
  /**
   * \brief
   * \param packet Packet to be routed
   * \param header Ipv6 header
   * \param idev Input interface Netdevice.
   */
  enum ForwardingType RouteInput6 (const Ptr<const Packet> &packet, const Ipv6Header &header,
    const Ptr<const NetDevice> &idev) const;
  /**
   * \brief Assign a mpls interface to the device
   * \param device NetDevice
   * \returns interface index
   */
  uint32_t AddInterface (const Ptr<NetDevice> &device);
  /**
   * \brief Get mpls interface by index
   * \param index interface index
   */
  Ptr<MplsInterface> GetInterface (uint32_t index) const;
  /**
   * \brief Get mpls interface for specified device
   * \returns MplsInterface
   */
  Ptr<MplsInterface> GetInterfaceForDevice (const Ptr<NetDevice> &device) const;
  /**
   * \brief Get count of mpls interfaces
   */
  uint32_t GetNInterfaces (void) const;

  void SetPerInterfaceLabelSpace (bool );
  void IsPerInterfaceLabelSpace

protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  void SetNode (Ptr<Node> node);

  static const uint32_t MAX_STACK_LOOPS = 16;
  Ptr<NetDevice> RouteMpls (Ptr<Packet> packet, MplsLabelStack &stack, int32_t inIfIndex) const;

  void IpForward (Ptr<Packet> packet, Ptr<NetDevice> odev, uint8_t ttl) const;
  void Ip6Forward (Ptr<Packet> packet, Ptr<NetDevice> odev, uint8_t ttl) const;
  void MplsForward (Ptr<Packet> packet, const MplsLabelStack &stack, const Header* ipHeader, Ptr<NetDevice> odev) const;

private:
  typedef std::vector<Ptr<MplsInterface> > MplsInterfaceVector;

  LabelSpace m_labelSpace
  Ptr<Node> m_node;
  MplsInterfaceVector m_interfaces;

  /* TracedCallback<const Ipv4Header &, Ptr<const Packet>, uint32_t> m_sendOutgoingTrace;
   TracedCallback<const Ipv4Header &, Ptr<const Packet>, DropReason, Ptr<Ipv4>, uint32_t> m_dropTrace;
  */
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS__H */
