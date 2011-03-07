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

#ifndef MPLS_ROUTING_PROTOCOL_H
#define MPLS_ROUTING_PROTOCOL_H

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
class MplsRoutingProtocol : public Object
{
public:
  /**
   * \enum MplsRoutingErrno
   * \brief routing errors
   */
  enum MplsRoutingErrno
  {
    ERROR_NOTERROR = 1,
    ERROR_IPROUTING,
    ERROR_NOROUTE
  };

  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  MplsRoutingProtocol ();
  virtual ~MplsRoutingProtocol ();

  void SetNode (Ptr<Node> node);
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                const Address &to, NetDevice::PacketType packetType);

  enum MplsRoutingErrno RouteInput (Ptr<const Packet> p, const Ipv4Header &header,
                                    Ptr<const NetDevice> idev) const;
  enum MplsRoutingErrno RouteInput6 (Ptr<const Packet> p, const Ipv6Header &header,
                                     Ptr<const NetDevice> idev) const;

  void AddInterface (Ptr<NetDevice> device);

  /**
   * \param entry FEC-to-NHLF or IML-to-NHLF entry
   * Add new entry for mpls switching
   */
  void AddLibEntry (Ptr<const MplsLibEntry> entry);
  /**
   * \param entry FEC-to-NHLFE or IML-to-NHLFE entry
   * Remove entry
   */
  void RemoveLibEntry (Ptr<const MplsLibEntry> entry);
  /**
   * \param ifIndex incoming interface
   * \param label label
   * Remove IML-to-NHLF entry
   */
  //void RemoveLibEntry (int32_t ifIndex, uint32_t label);
  /**
   * \param fec FEC
   * Remove FEC-to-NHLF entry
   */
  //void RemoveLibEntry (const ForwardEquivalenceClass &fec);
  /**
   * \param ifIndex incoming interface
   * Remove all FEC-to-NHLF or IML-to-NHLF entries binded to specified interface
   */
  //void RemoveLibEntry (int32_t ifIndex);

  Ptr<NetDevice> GetMplsDevice (int32_t ifIndex) const;
  int32_t GetMplsDeviceIndex (Ptr<const NetDevice> device) const;
  Ptr<NetDevice> GetDevice (int32_t ifIndex) const;
  void PrintLibEntries (std::ostream &os) const;

protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  static const uint32_t MAX_STACK_LOOPS = 16;
  Ptr<NetDevice> RouteMpls (Ptr<Packet> packet, MplsLabelStack &stack, int32_t inIfIndex) const;

  Ptr<const MplsLibEntry> GetLibEntry (int32_t ifIndex, uint32_t label) const;
  Ptr<const MplsLibEntry> GetLibEntry (int32_t ifIndex, Ptr<const Packet> p, const Ipv4Header &header) const;
  Ptr<const MplsLibEntry> GetLibEntry (int32_t ifIndex, Ptr<const Packet> p, const Ipv6Header &header) const;

  void MplsForward (Ptr<Packet> packet, const MplsLabelStack &stack, const Header* ipHeader,
                    Ptr<NetDevice> odev) const;
  void IpForward (Ptr<Packet> packet, Ptr<NetDevice> odev, uint8_t ttl) const;
  void Ip6Forward (Ptr<Packet> packet, Ptr<NetDevice> odev, uint8_t ttl) const;

  typedef std::vector<Ptr<NetDevice> > DeviceVector;
  typedef std::list<Ptr<const MplsLibEntry> > MplsLibEntryList;

  Ptr<Node>         m_node;
  MplsLibEntryList  m_entries;
  DeviceVector      m_devices;

  /* TracedCallback<const Ipv4Header &, Ptr<const Packet>, uint32_t> m_sendOutgoingTrace;
   TracedCallback<const Ipv4Header &, Ptr<const Packet>, DropReason, Ptr<Ipv4>, uint32_t> m_dropTrace;
  */
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_PROTOCOL_H */
