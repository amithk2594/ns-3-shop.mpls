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

#ifndef LDP_PROTOCOL_H
#define LDP_PROTOCOL_H

#include <list>

#include "ns3/object.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/mpls-routing-protocol.h"
#include "ns3/forwarding-equivalence-class.h"
#include "ns3/traced-callback.h"

#include "protocol-data-unit.h"
#include "ldp-peer.h"
#include "ldp-extension.h"

namespace ns3 {
namespace ldp {

using namespace mpls;

class LdpPeer;
class LdpExtension;

class LdpProtocol : public Object
{
public:
  static const uint16_t HELLO_MESSAGE;
  static const uint16_t LDP_PORT;

  static TypeId GetTypeId (void);

  LdpProtocol ();
  virtual ~LdpProtocol();

  /**
   * returns this router id (ldp id)
   */
  uint32_t GetRouterId (void) const;
  /**
   * \param time session KeepAlive time
   */
  void SetKeepAliveTime (uint16_t time);
  /**
   * \returns time session KeepAlive time
   */
  uint16_t GetKeepAliveTime (void) const;
  /**
   * \param interval hello interval
   */
  void SetHelloInterval (uint16_t interval);
  /**
   * \returns hello interval
   */
  uint16_t GetHelloInterval (void) const;
  /**
   * \param interval hello interval
   */
  void SetMaxPduLength (uint16_t length);
  /**
   * \returns hello interval
   */
  uint16_t GetMaxPduLength (void) const;
  /**
   * \param ifIndex net device index
   * \returns ldp session if exists
   */
  Ptr<LdpPeer> GetPeerForDeviceIfIndex (int32_t ifIndex) const;
  /**
   * \param device net device
   * \returns ldp session if exists
   */
  Ptr<LdpPeer> GetPeerForDevice (Ptr<const NetDevice> device) const;
  /**
   * \param interface ipv4 interface
   * \returns ldp session if exists
   */
  Ptr<LdpPeer> GetPeerForInterface (Ptr<const Ipv4Interface> interface) const;
  /**
   * \param address session address
   * \returns ldp session if exists
   */
  Ptr<LdpPeer> GetPeerForAddress (const Address &address) const;
  /**
   * \param extension
   * Register extension
   */
  void InsertExtension (Ptr<LdpExtension> extension);
  /**
   * \param extension
   * Remove extension
   */
  void RemoveExtension (Ptr<LdpExtension> extension);

  Ptr<MplsLibEntry> Bind (uint32_t inLabel, int32_t inIfIndex, uint32_t outLabel, int32_t outIfIndex);
  Ptr<MplsLibEntry> Bind (const ForwardingEquivalenceClass &fec, uint32_t outLabel, int32_t outIfIndex);
  void Unbind (Ptr<MplsLibEntry> entry);

protected:
  virtual void NotifyNewAggregate (void);
  virtual void DoDispose (void);

private:
  void Initialize (void);
  void SetNode (Ptr<Node> node);
  void SetIpv4 (Ptr<Ipv4L3Protocol> ipv4);
  void SetMpls (Ptr<MplsRoutingProtocol> mpls);
  void CancelEvents (void);
  Ptr<LdpPeer> FindPeer (const Address &address) const;

  bool HandleConnectionRequest (Ptr<Socket> socket, const Address& from) const;
  void HandleConnectionAccept (Ptr<Socket> socket, const Address& from) const;
  void HandlePeerMessage (Ptr<LdpPeer> peer, Ptr<const Message> message);
  //void HandlePeerError (Ptr<const LdpPeer> session, enum PduDecodingErrno errno);
  void HandlePeerClose (Ptr<const LdpPeer> peer);

  Ptr<Message> CreateHelloMessage (void) const;
  void HandleHelloRead (Ptr<Socket> socket);
  bool HandleHelloMessage (uint32_t ifIndex, const Ipv4Address &address, uint32_t routerId, Ptr<const Message> message);
  void SendHelloMessage (void);
  void ScheduleHelloMessage (void);
  Ptr<Socket> ConnectSocketTo (const Ipv4Address &address) const;

  typedef std::list<Ptr<LdpPeer> > PeerList;
  typedef std::list<Ptr<Socket> > SocketList;
  typedef std::list<Ptr<LdpExtension> > ExtensionList;

  Ptr<Node> m_node;
  Ptr<MplsRoutingProtocol> m_mpls;
  Ptr<Ipv4L3Protocol> m_ipv4;
  Ptr<Socket> m_tcpSocket;
  Ptr<PduReader> m_reader;
  uint32_t m_routerId;
  PeerList m_peers;
  SocketList m_sockets;
  ExtensionList m_extensions;
  uint16_t m_helloInterval;
  uint16_t m_keepAliveTime;
  uint16_t m_maxPduLength;
  EventId m_helloEvent;

  TracedCallback<Ptr<const LdpPeer>, Ptr<const Packet>, const Address &> m_rxTrace;
  TracedCallback<Ptr<const LdpPeer>, Ptr<const Packet>, const Address &> m_txTrace;
};

} // namespace ldp
} // namespace ns3

#endif /* LDP_PROTOCOL_H */
