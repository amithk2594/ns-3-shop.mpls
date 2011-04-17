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
#include "common-tlv.h"
#include "fec-tlv.h"
#include "status-tlv.h"
#include "lsp-tunnel.h"
#include "trunk-utilization.h"

NS_LOG_COMPONENT_DEFINE ("TrunkUtilization");

namespace ns3 {
namespace ldp {

NS_OBJECT_ENSURE_REGISTERED (TrunkUtilization);

const uint16_t TrunkUtilization::TE_PORT = 700;
const uint16_t TrunkUtilization::TUNNEL_CREATE_MESSAGE = 0x3f00;
const uint16_t TrunkUtilization::TUNNEL_DELETE_MESSAGE = 0x3f01;
const uint16_t TrunkUtilization::NOTIFICATION_MESSAGE = 0x0001;

TypeId
TrunkUtilization::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ldp::TrunkUtilization")
    .SetParent<Object> ()
    .AddConstructor <TrunkUtilization> ()
    .AddAttribute ("TeServerAddress", "Address of TE-Server",
                   Ipv4AddressValue (),
                   MakeIpv4AddressAccessor (&TrunkUtilization::m_address),
                   MakeIpv4AddressChecker ())
    ;
  return tid;
}

TrunkUtilization::TrunkUtilization ()
  : m_node (0),
    m_ldp (0),
    m_crldp (0),
    m_socket (0),
    m_reader (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Schedule (Seconds (0), &TrunkUtilization::Initialize, this);
}

TrunkUtilization::~TrunkUtilization ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TrunkUtilization::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = 0;
  m_ldp = 0;
  m_crldp = 0;
  m_socket = 0;
  m_reader = 0;

  Object::DoDispose ();
}

void
TrunkUtilization::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = GetObject<Node> ();
      if (node != 0)
        {
          m_node = node;
        }
    }

  if (m_crldp == 0)
    {
      Ptr<LdpConstraintBasedRouting> crldp = GetObject<LdpConstraintBasedRouting> ();
      if (crldp != 0)
        {
          m_crldp = crldp;
        }
    }

  if (m_ldp == 0)
    {
      Ptr<LdpProtocol> ldp = GetObject<LdpProtocol> ();
      if (ldp != 0)
        {
          m_ldp = ldp;
        }
    }

  Object::NotifyNewAggregate ();
}

void
TrunkUtilization::Initialize (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_node != 0, "TrunkUtilization: Bad node");
  NS_ASSERT_MSG (m_ldp != 0, "TrunkUtilization: LDP protocol should be installed");
  NS_ASSERT_MSG (m_crldp != 0, "TrunkUtilization: CR-LDP extension should be installed");

  if (m_address.IsEqual(Ipv4Address::GetZero ()))
    {
      NS_LOG_DEBUG ("TrunkUtilization: TE-Server address is not assigned.");
      return;
    }

  m_socket = Socket::CreateSocket (m_node, TcpSocketFactory::GetTypeId ());

  m_socket->Bind (InetSocketAddress (Ipv4Address::GetAny ()));
  m_socket->Connect (InetSocketAddress (m_address, TE_PORT));
  m_socket->SetRecvCallback (MakeCallback (&TrunkUtilization::HandleSocketRead, this));
  m_socket->SetCloseCallbacks (MakeCallback (&TrunkUtilization::HandleSocketClose, this),
                             MakeCallback (&TrunkUtilization::HandleSocketClose, this));

  m_reader = Create<PduReader> ();
}

void
TrunkUtilization::HandleSocketRead (Ptr<Socket> socket)
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
TrunkUtilization::HandleSocketClose (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  if (m_socket != 0)
    {
      m_socket->Close ();
      m_socket = 0;
    }
}

void
TrunkUtilization::HandleMessage (uint32_t ldpid, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << ldpid);
  uint16_t type = message->GetMessageType ();
  switch (type)
  {
    case TUNNEL_CREATE_MESSAGE:
      HandleTunnelCreateMessage (message);
      break;
    case TUNNEL_DELETE_MESSAGE:
      HandleTunnelDeleteMessage (message);
      break;
    default:
      // just ignore
      break;
  }
}

void
TrunkUtilization::HandleError (uint32_t ldpid, uint32_t errno)
{
  NS_LOG_FUNCTION (this << ldpid << errno);
  //m_errorCallback (this, errno);
}

void
TrunkUtilization::HandleTunnelCreateMessage (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  NS_LOG_DEBUG ("Receive tunnel create message from TE");

  Message::Iterator i = message->Begin ();
  Ptr<const FecTLV> fec = DynamicCast<const FecTLV> (*i++);
  if (fec == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel create message. FEC TLV expected.");
      return;
    }

  if (fec->GetNElements () == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel create message. FEC TLV has no elements.");
      return;
    }

  Ptr<const HostAddressFecElement> element = DynamicCast<const HostAddressFecElement> (fec->GetElement (0));
  if (!(Ipv4Address::IsMatchingType (element->GetAddress ())))
    {
      NS_LOG_DEBUG ("Drop tunnel create message. Ipv4 Host Address FEC element expected.");
      return;
    }

  Ipv4Address address = Ipv4Address::ConvertFrom (element->GetAddress ());

  Ptr<const ExplicitRouteTLV> ertlv = DynamicCast<const ExplicitRouteTLV> (*i++);
  if (ertlv == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel create message. ER-TLV expected.");
      return;
    }

  uint16_t lspid = m_crldp->CreateLspTunnel (ertlv, MakeCallback (&TrunkUtilization::CreateTunnelSuccess, this),
    MakeCallback (&TrunkUtilization::CreateTunnelFail, this));

  if (lspid == uint16_t(-1))
    {
      NS_LOG_DEBUG ("Drop tunnel create message. Could not create tunnel.");
      return;
    }

  m_requests.push_back (Request (lspid, message->GetMessageId (), address));
}

void
TrunkUtilization::CreateTunnelSuccess (uint16_t lspid, Ptr<const LspTunnel> tunnel)
{
  NS_LOG_FUNCTION (this << lspid);
  RequestList::iterator i = m_requests.begin ();
  for (; i != m_requests.end (); ++i)
    {
      if ((*i).lspid == lspid)
        {
          break;
        }
    }

  if (i == m_requests.end ())
    {
      NS_LOG_DEBUG ("Invalid tunnel id");
      return;
    }

  Ptr<LdpPeer> peer = m_ldp->GetPeerForDeviceIfIndex (tunnel->GetOutIfIndex ());
  if (peer == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel create message. Peer is gone");
    }
  else
    {
      NS_LOG_DEBUG ("Bind FEC " << (*i).address << " olabel=" << tunnel->GetOutLabel ()
                     << " oindex=" << tunnel->GetOutIfIndex ());
      peer->BindFec ((*i).address, tunnel->GetOutLabel (), tunnel->GetOutIfIndex ());
    }

  NS_LOG_DEBUG ("Send back notification message");

  Ptr<StatusTLV> status = Create<StatusTLV> ();
  status->SetStatusCode (0);
  Ptr<LspIdTLV> lsp = Create<LspIdTLV> ();
  lsp->SetLspId (lspid);
  Ptr<Message> message = Create<Message> (NOTIFICATION_MESSAGE);
  message->AddValue (status);
  message->AddValue (lsp);
  message->SetMessageId ((*i).msgid);
  Ptr<PduWriter> writer = Create<PduWriter> ();
  writer->AddMessage (message);
  m_socket->Send (writer->Write ());
  m_requests.erase (i);
}

void
TrunkUtilization::CreateTunnelFail (uint16_t lspid)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TrunkUtilization::HandleTunnelDeleteMessage (Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << message);

  NS_LOG_DEBUG ("Receive tunnel delete message from TE");

  Message::Iterator i = message->Begin ();

  Ptr<const LspIdTLV> lspid = DynamicCast<const LspIdTLV> (*i++);
  if (lspid == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel delete message. LSPID TLV expected.");
      return;
    }

  Ptr<const LspTunnel> tunnel = m_crldp->GetLspTunnel (lspid->GetLspId ());
  if (tunnel == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel delete message. Invalid tunnel id " << lspid->GetLspId ());
      return;
    }

  Ptr<LdpPeer> peer = m_ldp->GetPeerForDeviceIfIndex (tunnel->GetOutIfIndex ());
  if (peer == 0)
    {
      NS_LOG_DEBUG ("Drop tunnel delete message. No peer for device " << tunnel->GetOutIfIndex ());
      return;
    }

  peer->UnbindFec (tunnel->GetOutLabel (), tunnel->GetOutIfIndex ());

  m_crldp->DeleteLspTunnel (lspid->GetLspId ());
}

} // namespace ldp
} // namespace ns3
