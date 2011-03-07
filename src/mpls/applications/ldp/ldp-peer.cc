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

#include <algorithm>

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-host-address.h"

#include "ldp-status-codes.h"
#include "common-session-params-tlv.h"
#include "status-tlv.h"
#include "ldp-peer.h"

NS_LOG_COMPONENT_DEFINE ("LdpPeer");

namespace ns3 {
namespace ldp {

using namespace mpls;

const uint16_t LdpPeer::NOTIFICATION_MESSAGE   = 0x0001;
const uint16_t LdpPeer::INITIALIZATION_MESSAGE = 0x0200;
const uint16_t LdpPeer::KEEPALIVE_MESSAGE      = 0x0201;

LdpPeer::LdpPeer (Ptr<LdpProtocol> ldp)
  : m_ldp (ldp),
    m_socket (0),
    m_reader (0),
    m_state (NON_EXISTENT_STATE),
    m_holdTime (0),
    m_keepAliveTime (60),
    m_sendKeepAliveTime (-1),
    m_maxPduLength (0),
    m_routerId (0),
    m_messageId (0),
    m_labelSeqNum (16),
    m_ifIndex (-1),
    m_address (),
    m_active (false),
    m_availLabel (0x10),
    m_unavailLabel (0x10000)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT (m_ldp != 0);

  m_labelListIter = m_labels.begin ();
}

LdpPeer::~LdpPeer ()
{
  NS_LOG_FUNCTION (this);
  m_ldp = 0;
}

void
LdpPeer::Initialize (Ptr<Socket> socket, bool active)
{
  NS_LOG_FUNCTION (this << socket << active);

  NS_ASSERT_MSG (m_socket == 0, "LdpPeer::Initialize (): already initialized");

  NS_ASSERT_MSG (socket != 0, "LdpPeer::Initialize (): bad socket");

  NS_ASSERT_MSG (!m_address.IsInvalid(), "LdpPeer::Initialize (): bad transport address");

  m_reader = Create<PduReader> ();

  socket->SetRecvCallback (MakeCallback (&LdpPeer::HandlePeerRead, this));
  socket->SetCloseCallbacks (MakeCallback (&LdpPeer::HandlePeerClose, this),
                             MakeCallback (&LdpPeer::HandlePeerClose, this));

  if (active)
    {
      m_active = true;
      socket->SetConnectCallback (MakeCallback (&LdpPeer::HandleConnectionSuccess, this),
                                  MakeCallback (&LdpPeer::HandleConnectionFailed, this));
    }
  else
    {
      m_state = INITIALIZED_STATE;
    }

  ResetKeepAliveTimeout ();
  m_socket = socket;
}

uint32_t
LdpPeer::GetState (void) const
{
  return m_state;
}

void
LdpPeer::SetIfIndex (int32_t index)
{
  m_ifIndex = index;
}

int32_t
LdpPeer::GetIfIndex (void) const
{
  return m_ifIndex;
}

void
LdpPeer::SetAddress (const Address &address)
{
  m_address = address;
}

Address
LdpPeer::GetAddress (void) const
{
  return m_address;
}

void
LdpPeer::SetRouterId (uint32_t routerId)
{
  m_routerId = routerId;
}

uint32_t
LdpPeer::GetRouterId (void) const
{
  return m_routerId;
}

void
LdpPeer::SetHoldTime (uint16_t value)
{
  NS_LOG_FUNCTION (this << value);

  if (value == 0)
    {
      value = 15; // default value
    }

  m_holdTime = value;
  ResetHoldTimeout ();
}

uint16_t
LdpPeer::GetHoldTime (void) const
{
  return m_holdTime;
}

uint16_t
LdpPeer::GetKeepAliveTime (void) const
{
  return m_keepAliveTime;
}

uint16_t
LdpPeer::GetMaxPduLength (void) const
{
  return m_maxPduLength;
}

bool
LdpPeer::IsActiveRole (void) const
{
  return m_active;
}

void
LdpPeer::ResetHoldTimeout (void)
{
  Simulator::Cancel (m_helloTimeoutEvent);
  m_helloTimeoutEvent = Simulator::Schedule (Seconds(m_holdTime), &LdpPeer::HandleHoldTimeout, this);
}

void
LdpPeer::Reject (uint32_t rejectReason)
{
  NS_LOG_FUNCTION (this << std::hex << rejectReason << std::dec);

  m_socket->SetDataSentCallback (MakeNullCallback<void, Ptr<Socket>, uint32_t> ());
  Send (CreateNotificationMessage (rejectReason));
  ClosePeer ();
}

void
LdpPeer::Send (Ptr<Message> msg1)
{
  NS_LOG_FUNCTION (this << msg1);

  ResetSendKeepAliveTimeout ();
  Ptr<PduWriter> writer = Create<PduWriter> ();
  writer->SetLdpId (m_ldp->GetRouterId ());
  msg1->SetMessageId (++m_messageId);
  writer->AddMessage (msg1);
  m_socket->Send (writer->Write ());
}

void
LdpPeer::Send (Ptr<Message> msg1, Ptr<Message> msg2)
{
  NS_LOG_FUNCTION (this << msg1 << msg2);

  ResetSendKeepAliveTimeout ();
  Ptr<PduWriter> writer = Create<PduWriter> ();
  writer->SetLdpId (m_ldp->GetRouterId ());
  msg1->SetMessageId (++m_messageId);
  msg2->SetMessageId (++m_messageId);
  writer->AddMessage (msg1);
  writer->AddMessage (msg2);
  m_socket->Send (writer->Write ());
}

void
LdpPeer::Send (Ptr<Message> msg1, Ptr<Message> msg2, Ptr<Message> msg3)
{
  NS_LOG_FUNCTION (this << msg1 << msg2 << msg3);

  ResetSendKeepAliveTimeout ();
  Ptr<PduWriter> writer = Create<PduWriter> ();
  writer->SetLdpId (m_ldp->GetRouterId ());
  msg1->SetMessageId (++m_messageId);
  msg2->SetMessageId (++m_messageId);
  msg3->SetMessageId (++m_messageId);
  writer->AddMessage (msg1);
  writer->AddMessage (msg2);
  writer->AddMessage (msg3);
  m_socket->Send (writer->Write ());
}

uint32_t
LdpPeer::BindLabel (uint32_t outLabel, int32_t outIfIndex)
{
  NS_LOG_FUNCTION (this << outLabel << outIfIndex);

  LabelList::iterator i = AllocateLabel ();

  (*i).second = m_ldp->Bind ((*i).first, m_ifIndex, outLabel, outIfIndex);

  return (*i).first;
}

bool
LdpPeer::UnbindLabel (uint32_t label, uint32_t &outLabel, int32_t &outIfIndex)
{
  NS_LOG_FUNCTION (this << outLabel << outIfIndex);

  for (LabelList::iterator i = m_labels.begin (); i != m_labels.end (); ++i)
    {
      if ((*i).first == label)
        {
          Ptr<MplsLibEntry> entry = (*i).second;
          if (entry == 0)
            {
              return false;
            }

          outLabel = entry->GetOutLabel ();
          outIfIndex = entry->GetOutIfIndex ();
          m_ldp->Unbind (entry);
          m_labels.erase (i);
          return true;
        }
    }

  return false;
}

void
LdpPeer::BindFec (const Ipv4Address &fec, uint32_t outLabel, int32_t outIfIndex)
{
  NS_LOG_FUNCTION (this << fec << outLabel << outIfIndex);

  m_fecs.push_back (std::make_pair(fec, m_ldp->Bind (Ipv4HostAddress (fec), outLabel, outIfIndex)));
}

bool
LdpPeer::UnbindFec (const Ipv4Address &fec)
{
  NS_LOG_FUNCTION (this << fec);

  for (FecList::iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
    {
      if ((*i).first == fec)
        {
          Ptr<MplsLibEntry> entry = (*i).second;
          if (entry == 0)
            {
              return false;
            }

          m_ldp->Unbind (entry);
          m_fecs.erase (i);
          return true;
        }
    }
  return false;
}

bool
LdpPeer::UnbindFec (uint32_t outLabel, int32_t outIfIndex)
{
  NS_LOG_FUNCTION (this << outLabel << outIfIndex);

  for (FecList::iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
    {
      Ptr<MplsLibEntry> entry = (*i).second;
      if (entry->GetOutIfIndex () == outIfIndex && entry->GetOutLabel () == outLabel)
        {
          m_ldp->Unbind (entry);
          m_fecs.erase (i);
          return true;
        }
    }

  return false;
}

LdpPeer::LabelList::iterator
LdpPeer::AllocateLabel ()
{
  if (m_availLabel < m_unavailLabel)
    {
      uint32_t i = m_availLabel++;
      return m_labels.insert (m_labelListIter, std::make_pair(i, Ptr<MplsLibEntry> (0)));
    }

  NS_ASSERT_MSG (m_labels.size () < 0xffff0, "LdpConstraintBasedRouting::AllocateLspId (): "
                                              "label limit exceeded");

  m_availLabel = 0x10;
  m_labelListIter = m_labels.begin ();
  while (m_labelListIter != m_labels.end ())
    {
      m_unavailLabel = (*m_labelListIter).first;
      if (m_availLabel < m_unavailLabel)
        {
          return AllocateLabel ();
        }
      m_availLabel = m_unavailLabel + 1;
      m_labelListIter++;
    }

  m_unavailLabel = 0x10000;
  return AllocateLabel ();
}

void
LdpPeer::SetMessageCallback (MessageCallback cb)
{
  m_messageCallback = cb;
}

void
LdpPeer::SetCloseCallback (CloseCallback cb)
{
  m_closeCallback = cb;
}

/*void
LdpPeer::SetErrorCallback (ErrorCallback cb)
{
  m_errorCallback = cb;
}*/

Ptr<Message>
LdpPeer::CreateInitializationMessage () const
{
  Ptr<CommonSessionParamsTLV> params = Create<CommonSessionParamsTLV> ();
  params->SetKeepAliveTime (m_ldp->GetKeepAliveTime ());
  params->SetMaxPduLength (m_ldp->GetMaxPduLength ());
  params->SetLdpIdentifier (m_routerId);
  Ptr<Message> message = Create<Message> (INITIALIZATION_MESSAGE);
  message->AddValue (params);
  return message;
}

Ptr<Message>
LdpPeer::CreateKeepAliveMessage (void) const
{
  return Create<Message> (KEEPALIVE_MESSAGE);
}

Ptr<Message>
LdpPeer::CreateNotificationMessage (uint32_t statusCode, uint32_t messageId, uint32_t messageType) const
{
  Ptr<StatusTLV> status = Create<StatusTLV> ();
  status->SetStatusCode (statusCode);
  if (messageId != 0)
    {
      status->SetMessageId (messageId);
    }

  if (messageType != 0)
    {
      status->SetMessageType (messageType);
    }

  Ptr<Message> message = Create<Message> (NOTIFICATION_MESSAGE);
  message->AddValue (status);
  return message;
}

void
LdpPeer::HandleConnectionSuccess (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  m_state = INITIALIZED_STATE;
  m_socket->SetDataSentCallback (MakeCallback (&LdpPeer::HandleSendInitializationMessage, this));

  Send (CreateInitializationMessage ());
}

void
LdpPeer::HandleConnectionFailed (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  ClosePeer ();
}

void
LdpPeer::HandlePeerRead (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  uint8_t buffer[1024];

  while (packet = socket->RecvFrom (1024, 0, from))
    {
      uint32_t packetSize = packet->GetSize();
      if (packetSize == 0)
        {
          break;
        }

      packet->CopyData (buffer, packetSize);
      m_reader->Feed (buffer, packetSize);
      Ptr<const Message> message;
      while (message = m_reader->GetNextMessage ())
        {
          HandleMessage (m_reader->GetLdpId (), message);
        }

      if (m_reader->GetLastError () != 0)
        {
          HandleError (m_reader->GetLdpId (), m_reader->GetLastError ());
          break;
        }
    }
}

void
LdpPeer::HandlePeerClose (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  if (m_socket != 0)
    {
      ClosePeer ();
    }

  m_closeCallback (this);
}

void
LdpPeer::HandleSendInitializationMessage (Ptr<Socket> socket, uint32_t bytes)
{
  NS_LOG_FUNCTION (this << socket << bytes);

  m_state = OPENSENT_STATE;
  socket->SetDataSentCallback (MakeNullCallback<void, Ptr<Socket>, uint32_t> ());
}

void
LdpPeer::HandleHoldTimeout (void)
{
  NS_LOG_FUNCTION (this);

  ClosePeer ();
}

void
LdpPeer::HandleKeepAliveTimeout (void)
{
  NS_LOG_FUNCTION (this);

  if (m_state == OPERATIONAL_STATE)
    {
      Reject (LdpStatusCodes::KEEPALIVE_TIMER_EXPIRED);
    }
  else
    {
      RejectNAK (LdpStatusCodes::KEEPALIVE_TIMER_EXPIRED);
    }
}

void
LdpPeer::ResetSendKeepAliveTimeout (void)
{
  Simulator::Cancel (m_sendKeepAliveTimeoutEvent);
  m_sendKeepAliveTimeoutEvent = Simulator::Schedule (Seconds(m_sendKeepAliveTime), &LdpPeer::SendKeepAliveMessage, this);
}

void
LdpPeer::ResetKeepAliveTimeout (void)
{
  Simulator::Cancel (m_keepAliveTimeoutEvent);
  m_keepAliveTimeoutEvent = Simulator::Schedule (Seconds(m_keepAliveTime), &LdpPeer::HandleKeepAliveTimeout, this);
}

void
LdpPeer::HandleMessage (uint32_t ldpid, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << ldpid << m_routerId);

  uint32_t rejectReason = LdpStatusCodes::SESSION_REJECTED_NO_HELLO;
  if (ldpid == m_routerId)
    {
      ResetKeepAliveTimeout ();

      switch (m_state)
        {
          case INITIALIZED_STATE:
            if (!m_active)
              {
                rejectReason = HandleInitializedStateMessages (message);
              }
            break;

          case OPENREC_STATE:
            rejectReason = HandleOpenRecStateMessages (message);
            break;

          case OPENSENT_STATE:
            rejectReason = HandleOpenSentStateMessages (message);
            break;

          case OPERATIONAL_STATE:
            HandleOperationalStateMessages (message);
            return;

          default:
            break;
        }
    }

  if (rejectReason != 0)
    {
      RejectNAK (rejectReason);
    }
}

void
LdpPeer::HandleError (uint32_t ldpid, uint32_t errno)
{
  NS_LOG_FUNCTION (this << ldpid << errno);
  //m_errorCallback (this, errno);
}

void
LdpPeer::ClosePeer (void)
{
  NS_LOG_FUNCTION (this << m_socket);

  m_state = NON_EXISTENT_STATE;

  if (m_socket != 0)
    {
      m_socket->SetRecvCallback (MakeNullCallback <void, Ptr<Socket> > ());
      m_socket->Close ();
      m_socket = 0;
    }

  Simulator::Cancel (m_helloTimeoutEvent);
  Simulator::Cancel (m_sendKeepAliveTimeoutEvent);
  Simulator::Cancel (m_keepAliveTimeoutEvent);

  m_reader = 0;
}

uint32_t
LdpPeer::HandleInitializedStateMessages (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  uint32_t code = HandleInitializationMessage (message);
  if (code != 0)
    {
      return code;
    }

  Send (CreateInitializationMessage (), CreateKeepAliveMessage ());

  m_state = OPENREC_STATE;
  return 0;
}

uint32_t
LdpPeer::HandleOpenRecStateMessages (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  if (message->GetMessageType () != KEEPALIVE_MESSAGE)
    {
      return LdpStatusCodes::SESSION_REJECTED_NO_HELLO;
    }

  m_sendKeepAliveTimeoutEvent = Simulator::Schedule (Seconds(m_sendKeepAliveTime), &LdpPeer::SendKeepAliveMessage, this);
  m_state = OPERATIONAL_STATE;
  return 0;
}

uint32_t
LdpPeer::HandleOpenSentStateMessages (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  uint32_t code = HandleInitializationMessage (message);
  if (code != 0)
    {
      return code;
    }

  Send (CreateKeepAliveMessage ());

  m_state = OPENREC_STATE;

  return 0;
}

void
LdpPeer::HandleOperationalStateMessages (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  if (message->GetMessageType () == KEEPALIVE_MESSAGE)
    {
      return;
    }

  if (!m_messageCallback.IsNull ())
    {
      m_messageCallback (this, message);
    }
}

uint32_t
LdpPeer::HandleInitializationMessage (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  if (message->GetMessageType () != INITIALIZATION_MESSAGE)
    {
      return LdpStatusCodes::SESSION_REJECTED_NO_HELLO;
    }

  Message::Iterator i = message->Begin ();
  Ptr<const CommonSessionParamsTLV> params = DynamicCast<const CommonSessionParamsTLV> (*i++);
  if (params == 0)
    {
      NS_LOG_DEBUG ("LdpProtocol::HandleInitializeMessages (): invalid initialization message");
      return LdpStatusCodes::SESSION_REJECTED_NO_HELLO;
    }

  m_keepAliveTime = std::min (m_ldp->GetKeepAliveTime (), params->GetKeepAliveTime ());

  if (m_keepAliveTime < 5)
    {
      NS_LOG_DEBUG ("LdpProtocol::HandleInitializeMessages (): bad keep-alive time");
      return LdpStatusCodes::SESSION_REJECTED_BAD_KEEPALIVE_TIME;
    }

  m_sendKeepAliveTime = m_keepAliveTime * 2 / 3; // send KeepAlive with interval = 2/3 * keepAliveTime

  m_maxPduLength = std::min (m_ldp->GetMaxPduLength (), params->GetMaxPduLength ());
  if (m_maxPduLength < 256 || m_maxPduLength > 4096)
    {
      NS_LOG_DEBUG ("LdpProtocol::HandleInitializeMessages (): bad max pdu size");
      return LdpStatusCodes::SESSION_REJECTED_PARAMETER_MAX_PDU_LENGTH;
    }

  ResetKeepAliveTimeout ();
  return 0;
}

void
LdpPeer::SendKeepAliveMessage (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ());

  Send (CreateKeepAliveMessage ());
}

void
LdpPeer::RejectNAK (uint32_t rejectReason)
{
  NS_LOG_FUNCTION (this << std::hex << rejectReason << std::dec);

  // XXX: advanced handling of NAK

  Reject (rejectReason);
}

} // namespace ldp
} // namespace ns3
