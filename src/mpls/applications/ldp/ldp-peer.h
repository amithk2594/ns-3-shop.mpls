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

#ifndef LDP_PEER_H
#define LDP_PEER_H

#include <list>

#include "ns3/object.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/mpls-lib-entry.h"

#include "protocol-data-unit.h"
#include "ldp-protocol.h"

namespace ns3 {
namespace ldp {

using namespace mpls;

class LdpProtocol;

class LdpPeer: public SimpleRefCount<LdpPeer>
{
public:
  static const uint16_t NOTIFICATION_MESSAGE;
  static const uint16_t INITIALIZATION_MESSAGE;
  static const uint16_t KEEPALIVE_MESSAGE;

  // session state
  enum LdpPeerState
  {
    NON_EXISTENT_STATE = 0,
    INITIALIZED_STATE,
    OPENSENT_STATE,
    OPENREC_STATE,
    OPERATIONAL_STATE
  };

  LdpPeer (Ptr<LdpProtocol> ldp);
  virtual ~LdpPeer ();

  /**
   * \param socket session socket
   * \param active true if initialize in active role
   * Initialize session
   */
  void Initialize (Ptr<Socket> socket, bool active);
  /**
   * \returns session state
   */
  uint32_t GetState (void) const;
  /**
   * \param ifIndex Interface index
   */
  void SetIfIndex (int32_t index);
  /**
   * \param ifIndex Interface index
   */
  int32_t GetIfIndex (void) const;
  /**
   * \param address transport address
   */
  void SetAddress (const Address &address);
  /**
   * \returns transport address
   */
  Address GetAddress (void) const;
  /**
   * \param routerId router identifier
   */
  void SetRouterId (uint32_t routerId);
  /**
   * \returns router identifier
   */
  uint32_t GetRouterId (void) const;
  /**
   * \param value session hold time
   */
  void SetHoldTime (uint16_t value);
  /**
   * \returns session hold time
   */
  uint16_t GetHoldTime (void) const;
  /**
   * \returns default keep alive time
   */
  uint16_t GetKeepAliveTime (void) const;
  /**
   * \returns default maximum pdu length
   */
  uint16_t GetMaxPduLength (void) const;
  /**
   * \returns true if session in active role
   */
  bool IsActiveRole (void) const;
  /**
   * \brief reset hold timeout
   */
  void ResetHoldTimeout (void);
  /**
   * \brief send messages to the peer
   */
  void Send (Ptr<Message> msg1);
  /**
   * \brief send messages to the peer
   */
  void Send (Ptr<Message> msg1, Ptr<Message> msg2);
  /**
   * \brief send messages to the peer
   */
  void Send (Ptr<Message> msg1, Ptr<Message> msg2, Ptr<Message> msg3);

  uint32_t BindLabel (uint32_t outLabel = -1, int32_t outIfIndex = -1);
  bool UnbindLabel (uint32_t label, uint32_t &outLabel, int32_t &outIfIndex);
  void BindFec (const Ipv4Address &fec, uint32_t outLabel, int32_t outIfIndex);
  bool UnbindFec (const Ipv4Address &fec);
  bool UnbindFec (uint32_t outLabel, int32_t outIfIndex);
  /**
   * \param rejectReason reject reason
   * \brief Soft session reject with the notification message
   */
  void Reject (uint32_t rejectReason);

  typedef Callback<void, Ptr<LdpPeer>, Ptr<const Message> > MessageCallback;
  typedef Callback<void, Ptr<const LdpPeer> > CloseCallback;

  /**
   * \param cb message callback
   */
  void SetMessageCallback (MessageCallback cb);
  /**
   * \param cb peer close callback
   */
  void SetCloseCallback (CloseCallback cb);

private:
  typedef std::list<std::pair<uint32_t, Ptr<MplsLibEntry> > > LabelList;
  typedef std::list<std::pair<Ipv4Address, Ptr<MplsLibEntry> > > FecList;
  void DeallocateLabel (uint32_t label);
  LabelList::iterator AllocateLabel (void);

  Ptr<Message> CreateInitializationMessage (void) const;
  Ptr<Message> CreateKeepAliveMessage (void) const;
  Ptr<Message> CreateNotificationMessage (uint32_t statusCode, uint32_t messageId = 0,
                                          uint32_t messageType = 0) const;
  void HandleConnectionSuccess (Ptr<Socket> socket);
  void HandleConnectionFailed (Ptr<Socket> socket);
  void HandlePeerRead (Ptr<Socket> socket);
  void HandlePeerClose (Ptr<Socket> socket);
  void HandleSendInitializationMessage (Ptr<Socket> socket, uint32_t bytes);
  void HandleHoldTimeout (void);
  void HandleKeepAliveTimeout (void);
  void ResetKeepAliveTimeout (void);
  void ResetSendKeepAliveTimeout (void);
  /**
   * \param rejectReason reject reason
   * \brief Reject session due to NAK.
   */
  void RejectNAK (uint32_t rejectReason);
  /**
   * \brief send keep-alive message to peer
   */
  void SendKeepAliveMessage (void);
  /**
   * \brief incoming message handler
   */
  void HandleMessage (uint32_t ldpid, Ptr<const Message> message);
  /**
   * \brief error handler
   */
  void HandleError (uint32_t ldpid, uint32_t errno);
  /**
   * \brief close peer and
   */
  void ClosePeer (void);

  uint32_t HandleInitializedStateMessages (Ptr<const Message> message);
  uint32_t HandleInitializationMessage (Ptr<const Message> message);
  uint32_t HandleOpenRecStateMessages (Ptr<const Message> message);
  uint32_t HandleOpenSentStateMessages (Ptr<const Message> message);
  void HandleOperationalStateMessages (Ptr<const Message> message);

  Ptr<LdpProtocol> m_ldp;
  Ptr<Socket> m_socket;
  Ptr<PduReader> m_reader;
  LdpPeerState m_state;
  uint16_t m_holdTime;
  uint16_t m_keepAliveTime;
  uint16_t m_sendKeepAliveTime;
  uint16_t m_maxPduLength;
  uint32_t m_routerId;
  uint32_t m_messageId;
  uint32_t m_labelSeqNum;
  int32_t m_ifIndex;
  Address m_address;
  bool m_active;
  uint32_t m_availLabel;
  uint32_t m_unavailLabel;
  LabelList m_labels;
  LabelList::iterator m_labelListIter;
  FecList m_fecs;

  EventId m_helloTimeoutEvent;
  EventId m_keepAliveTimeoutEvent;
  EventId m_sendKeepAliveTimeoutEvent;

  MessageCallback m_messageCallback;
  CloseCallback m_closeCallback;
  //ErrorCallback   m_errorCallback;

  //friend class LdpProtocol;
};

} // namespace ldp
} // namespace ns3

#endif /* LDP_PEER_H */
