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
#include "ns3/ipv6-header.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/traced-callback.h"

#include "mpls.h"
#include "mpls-label.h"
#include "mpls-interface.h"
#include "mpls-label-stack.h"
#include "mpls-forwarding-information.h"
#include "mpls-incoming-label-map.h"
#include "mpls-fec-to-nhlfe.h"
#include "mpls-nhlfe.h"
#include "mpls-ilm-table.h"
#include "mpls-ftn-table.h"
#include "mpls-ipv4-protocol.h"
#include "mpls-packet-demux.h"
#include "mpls-traces.h"

namespace ns3 {
namespace mpls {

class Nhlfe;
class ForwardingInformation;
class Ipv4Protocol;

/**
 * \ingroup mpls
 */
class MplsProtocol : public Mpls
{
public:
  static TypeId GetTypeId (void);

  MplsProtocol ();
  virtual ~MplsProtocol ();

  /**
   * @brief Set new ILM table
   */
  void SetIlmTable (const Ptr<IlmTable> &ilmTable);
  /**
   * @brief Get ILM table
   */
  Ptr<IlmTable> GetIlmTable (void) const;
  /**
   * @brief Set new FTN table
   */
  void SetFtnTable (const Ptr<FtnTable> &ftnTable);
  /**
   * @brief Get Ftn table
   */
  Ptr<FtnTable> GetFtnTable (void) const;
  /**
   * @param device device to add to the list of Mpls interfaces
   * @return the Mpls interface
   */
  Ptr<Interface> AddInterface (const Ptr<NetDevice> &device);
  /**
   * @brief Get Mpls interface by index
   * @param index Mpls interface index
   */
  Ptr<Interface> GetInterface (int32_t index) const;
  /**
   * @brief Get Mpls interface for specified device
   * @return Mpls interface
   */
  Ptr<Interface> GetInterfaceForDevice (const Ptr<const NetDevice> &device) const;
  /**
   * @brief The number of Mpls interfaces added
   */
  uint32_t GetNInterfaces (void) const;
  /**
   * @brief Lower layer calls this method after calling L3Demux::Lookup
   */
  void ReceiveMpls (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                    const Address &to, NetDevice::PacketType packetType);  
  /**
   * @brief Unlabeled ipv4 packet entry point
   */
  bool ReceiveIpv4 (const Ptr<Packet> &packet, const Ipv4Header &header, const Ptr<const NetDevice> &device);
  /**
   * @brief Unlabeled ipv4 packet entry point
   */
  bool ReceiveIpv6 (const Ptr<Packet> &packet, const Ipv6Header &header, const Ptr<const NetDevice> &device);
  /**
   * @brief New interface for specified device is available
   */
  void NotifyNewInterface (const Ptr<NetDevice> &device);

protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();

private:
  typedef std::vector<Ptr<Interface> > InterfaceList;

  void MplsForward (const Ptr<Packet> &packet, const Ptr<ForwardingInformation> &fwd, LabelStack &stack, int8_t ttl);
  Ptr<IncomingLabelMap> LookupIlm (Label label, int32_t interface);
  Ptr<FecToNhlfe> LookupFtn (PacketDemux& demux);
  bool RealMplsForward (const Ptr<Packet> &packet, const Nhlfe &nhlfe, LabelStack &stack, int8_t ttl,
                          const Ptr<Interface> &outInterface);
  void IpForward (const Ptr<Packet> &packet, uint8_t ttl, const Ptr<NetDevice> &outDev, const Ptr<Ipv4Route> &route);

  Ptr<Ipv4Route> GetNextHopRoute (const Address &address) const;

  Ptr<Node> m_node;
  Ptr<mpls::Ipv4Protocol> m_ipv4;
  Ptr<IlmTable> m_ilmTable;
  Ptr<FtnTable> m_ftnTable;

  InterfaceList m_interfaces;
  bool m_interfaceAutoInstall;

  PacketDemux m_demux;
  
  traces::TxTracedCallback m_txTrace;
  traces::RxTracedCallback m_rxTrace;
  traces::DropTracedCallback m_dropTrace;
};


class IpProbe : public Header
{
public:  
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  /**
   * @brief Create an empty object
   */ 
  IpProbe ();
  /**
   * @brief Destructor
   */ 
  virtual ~IpProbe ();
  /**
   * @brief Returns the IP protocol version
   */ 
  static uint8_t GetIpVersion (const Ptr<Packet> &packet);
  
  // Functions defined in base class Header
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const; 
  
private:
  uint8_t m_version;
};


} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_PROTOCOL_H */
