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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/inet-socket-address.h"
#include "protocol-data-unit.h"
#include "common-cr-tlv.h"
#include "fec-tlv.h"
#include "status-tlv.h"

#include "te-server.h"

NS_LOG_COMPONENT_DEFINE ("TeServer");

namespace ns3 {
namespace ldp {

NS_OBJECT_ENSURE_REGISTERED (TeServer);

const uint16_t TeServer::TE_PORT = 700;
const uint16_t TeServer::TUNNEL_CREATE_MESSAGE = 0x3f00;
const uint16_t TeServer::TUNNEL_DELETE_MESSAGE = 0x3f01;
const uint16_t TeServer::NOTIFICATION_MESSAGE = 0x0001;

TypeId
TeServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ldp::TeServer")
    .SetParent<Object> ()
    .AddConstructor <TeServer> ()
    ;
  return tid;
}

TeServer::TeServer ()
  : m_node (0),
    m_socket (0),
    m_reader (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Schedule (Seconds (0), &TeServer::Initialize, this);
}

TeServer::~TeServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TeServer::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = 0;
  m_socket = 0;
  m_reader = 0;

  Object::DoDispose ();
}

void
TeServer::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = GetObject<Node> ();
      if (node != 0)
        {
          m_node = node;
        }
    }

  Object::NotifyNewAggregate ();
}

void
TeServer::SetOperationSuccessCallback (OperationCallback cb)
{
  m_scb = cb;
}

void
TeServer::SetOperationFailCallback (OperationCallback cb)
{
  m_fcb = cb;
}

void
TeServer::Initialize (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_node != 0, "TeServer: Bad node");

  m_socket = Socket::CreateSocket (m_node, TcpSocketFactory::GetTypeId ());
  m_socket->Bind (InetSocketAddress (Ipv4Address::GetAny (), TE_PORT));
  m_socket->SetAcceptCallback (MakeCallback (&TeServer::HandleConnectionRequest, this),
                                    MakeCallback (&TeServer::HandleConnectionAccept, this));
  m_socket->Listen ();
  m_reader = Create<PduReader> ();
}

bool
TeServer::HandleConnectionRequest (Ptr<Socket> socket, const Address& from)
{
  NS_LOG_FUNCTION (this << socket << from);

  if (!InetSocketAddress::IsMatchingType (from))
    {
      return false;
    }

  return true;
}

void
TeServer::HandleConnectionAccept (Ptr<Socket> socket, const Address& from)
{
  NS_LOG_FUNCTION (this << socket << from);

  InetSocketAddress addr = InetSocketAddress::ConvertFrom (from);
  NS_LOG_DEBUG ("Connection from " << addr.GetIpv4 ());
  socket->SetRecvCallback (MakeCallback (&TeServer::HandleSocketRead, this));
  m_sockets.push_back (std::make_pair (addr.GetIpv4 (), socket));
}

void
TeServer::HandleSocketRead (Ptr<Socket> socket)
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

      NS_LOG_FUNCTION (this << InetSocketAddress::ConvertFrom (from).GetIpv4 () << " " << packetSize);

      packet->CopyData (buffer, packetSize);
      m_reader->Feed (buffer, packetSize);
      Ptr<const Message> message;
      while (message = m_reader->GetNextMessage ())
        {
          HandleMessage (socket, m_reader->GetLdpId (), message);
        }

      if (m_reader->GetLastError () != 0)
        {
          HandleError (socket, m_reader->GetLdpId (), m_reader->GetLastError ());
          break;
        }
    }
}

void
TeServer::HandleMessage (Ptr<Socket> socket, uint32_t ldpid, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << ldpid);

  uint16_t type = message->GetMessageType ();
  switch (type)
  {
    case NOTIFICATION_MESSAGE:
      HandleNotificationMessage (socket, ldpid, message);
      break;
    default:
      // just ignore
      break;
  }
}

void
TeServer::HandleError (Ptr<Socket> socket, uint32_t ldpid, uint32_t errno)
{
  NS_LOG_FUNCTION (this << ldpid << errno);
  //m_errorCallback (this, errno);
}

void
TeServer::HandleNotificationMessage (Ptr<Socket> socket, uint32_t ldpid, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  NS_LOG_DEBUG ("Receive notification message from " << Ipv4Address (ldpid) << " " << message->GetMessageId ());

  Ptr<Request> request = m_requests.Get (message->GetMessageId ());

  if (request == 0 || (*request).socket != socket)
    {
      NS_LOG_DEBUG ("Drop notification message. Not for us.");
      return;
    }

  m_requests.Remove (message->GetMessageId ());

  Message::Iterator i = message->Begin ();
  Ptr<const StatusTLV> status = DynamicCast<const StatusTLV> (*i++);
  if (status == 0)
    {
      NS_LOG_DEBUG ("Drop notification message. Status TLV expected.");
      return;
    }

  Ptr<const LspIdTLV> lspid = DynamicCast<const LspIdTLV> (*i++);
  if (lspid == 0)
    {
      NS_LOG_DEBUG ("Drop notification message. LSPID TLV expected.");
      return;
    }

  Ptr<Trunk> trunk = (*request).trunk;
  if (trunk->lsp1 == uint16_t(-1))
    {
      trunk->sock1 = (*request).socket;
      trunk->lsp1 = lspid->GetLspId ();
      return;
    }

  trunk->sock2 = (*request).socket;
  trunk->lsp2 = lspid->GetLspId ();
  if (!m_scb.IsNull ())
    {
      m_scb (trunk->id);
    }
}

uint32_t
TeServer::CreateTrunk (const std::vector<Ipv4Address> &pathTo, const std::vector<Ipv4Address> &pathFrom)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Trunk> trunk = Create<Trunk> ();
  trunk->id = m_trunks.Allocate (trunk);
  SendTunnelCreateMessage (pathTo, trunk);
  SendTunnelCreateMessage (pathFrom, trunk);
  return trunk->id;
}

void
TeServer::DeleteTrunk (uint32_t trunkId)
{
  NS_LOG_FUNCTION (trunkId);
  Ptr<Trunk> trunk = m_trunks.Deallocate (trunkId);
  SendTunnelDeleteMessage (trunk->sock1, trunk->lsp1);
  SendTunnelDeleteMessage (trunk->sock2, trunk->lsp2);
}

void
TeServer::SendTunnelCreateMessage (const std::vector<Ipv4Address> &path, Ptr<TeServer::Trunk> trunk)
{
  Ptr<PduWriter> writer = Create<PduWriter> ();

  std::vector<Ipv4Address>::const_iterator i = path.begin ();
  Ptr<Socket> socket = GetSocket (*i++);

  NS_ASSERT_MSG (socket != 0, "Invalid socket address " << path.front ());

  Ptr<ExplicitRouteTLV> route = Create <ExplicitRouteTLV> ();
  uint32_t n = path.size () - 2;
  while (n > 0)
    {
      Ptr<Ipv4ExplicitRouteHopTLV> hop = Create<Ipv4ExplicitRouteHopTLV> ();
      hop->SetAddress (*i++);
      route->AddRouteHop (hop);
      n--;
    }

  Ptr<Request> request = Create<Request> (socket, trunk);
  uint32_t msgId = m_requests.Add (request);

  Ptr<FecTLV> fec = HostAddressFecElement::CreateFecTLV (*i);
  Ptr<Message> message = Create<Message> (TUNNEL_CREATE_MESSAGE);
  message->AddValue (fec);
  message->AddValue (route);
  message->SetMessageId (msgId);
  writer->AddMessage (message);

  NS_LOG_DEBUG ("Send tunnel create message to " << path.front () << " " << msgId);
  socket->Send (writer->Write ());
}

void
TeServer::SendTunnelDeleteMessage (Ptr<Socket> socket, uint32_t lsp)
{
  Ptr<PduWriter> writer = Create<PduWriter> ();
  Ptr<Message> message = Create<Message> (TUNNEL_DELETE_MESSAGE);
  Ptr<LspIdTLV> lspid = Create<LspIdTLV> (false, lsp, 0);
  message->AddValue (lspid);
  writer->AddMessage (message);

  socket->Send (writer->Write ());
}

Ptr<Socket>
TeServer::GetSocket (const Ipv4Address &address)
{
  for (SocketList::const_iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      if ((*i).first == address)
        {
          return (*i).second;
        }
    }

  return 0;
}

} // namespace ldp
} // namespace ns3
