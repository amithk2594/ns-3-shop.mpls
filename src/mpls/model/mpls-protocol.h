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

#ifndef MPLS_PROTOCOL_H
#define MPLS_PROTOCOL_H

#include <ostream>
#include <vector>

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

#include "mpls-interface.h"
#include "mpls-label-stack.h"
#include "mpls-incoming-label-map.h"
#include "mpls-nhlfe.h"
#include "mpls-ipv4-protocol.h"


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
  
  /**
   * @enum DropReason
   * @brief Reason why a packet has been dropped.
   */
  enum DropReason 
   {
     DROP_TTL_EXPIRED = 1, /**< Packet TTL has expired */
     DROP_NO_ROUTE, /**< No route to host */
     DROP_INTERFACE_DOWN, /**< Interface is down so can not send packet */
     DROP_ROUTE_ERROR, /**< Route error */
   };
    
  /**
   * @brief Set new ILM table
   */
  void SetIlmTable (const Ptr<IlmTable> &ilmTable);
  /**
   * @brief Get ILM table
   */
  Ptr<IlmTable> GetIlmTable (viod) const;
  /**
   * @brief Set new FTN table
   */
  void SetFtnTable (const Ptr<FtnTable> &ftnTable);
  /**
   * @brief Get Ftn table
   */
  Ptr<FtnTable> GetFtnTable (viod) const;
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
   * @brief Get route for the next-hop
   * @return Ipv4 route
   */
  Ptr<Ipv4Route> GetNextHopRoute (const Address &address) const;
  /**
   * @brief The number of Mpls interfaces added
   */
  uint32_t GetNInterfaces (void) const;
  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   */
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                    const Address &to, NetDevice::PacketType packetType);
  /**
   * Forward packet according to forwarding information
   */
  void MplsForward (Ptr<Packet> &packet, const Ptr<ForwardingInformation> &fwd, LabelStack &stack, int8_t ttl);
  
protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  typedef std::vector<Ptr<MplsInterface> > MplsInterfaceList;
  
  Ptr<IncomingLabelMap> LookupIlm (Label label, uint32_t interface);
  bool RealMplsForward (Ptr<Packet> &packet, const Nhlfe &nhlfe, LabelStack &stack, int8_t ttl, 
                          Ptr<MplsInterface> &outInterface);
  void IpForward (Ptr<Packet> &packet, uint8_t ttl, Ptr<NetDevice> &outDev, Ptr<Ipv4Route> route);
  
  Ptr<Node> m_node;
  Ptr<MplsIpv4Protocol> m_ipv4;
  Ptr<IlmTable> m_ilmTable;
  
  MplsInterfaceList m_interfaces;
  
  TracedCallback<Ptr<const Packet>, uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, uint32_t> m_rxTrace;
  TracedCallback<Ptr<const Packet>, DropReason, uint32_t> m_dropTrace;
  
  friend class MplsIpv4Routing;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_PROTOCOL_H */
