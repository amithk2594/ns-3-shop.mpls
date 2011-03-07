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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/udp-socket.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/loopback-net-device.h"
#include "ns3/ipv4-host-address.h"

#include "ldp-protocol.h"
#include "common-hello-params-tlv.h"

NS_LOG_COMPONENT_DEFINE ("LdpProtocol");

namespace ns3 {
namespace ldp {

NS_OBJECT_ENSURE_REGISTERED (LdpProtocol);

const uint16_t LdpProtocol::HELLO_MESSAGE = 0x0100;
const uint16_t LdpProtocol::LDP_PORT = 646;

TypeId
LdpProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ldp::LdpProtocol")
    .SetParent<Object> ()
    .AddConstructor<LdpProtocol> ()
    .AddAttribute ("HelloInterval", "LDP Hello Message interval",
                   UintegerValue (15),
                   MakeUintegerAccessor (&LdpProtocol::m_helloInterval),
                   MakeUintegerChecker<uint16_t> (1, 32767))
    .AddAttribute ("MaxPduLength", "Maximum PDU length",
                   UintegerValue (512),
                   MakeUintegerAccessor (&LdpProtocol::m_maxPduLength),
                   MakeUintegerChecker<uint16_t> (256, 4096))
    .AddAttribute ("KeepAliveTime", "LDP session KeepAlive time",
                   UintegerValue (20),
                   MakeUintegerAccessor (&LdpProtocol::m_keepAliveTime),
                   MakeUintegerChecker<uint16_t> (1))
    ;
  return tid;
}

LdpProtocol::LdpProtocol ()
  : m_node (0),
    m_mpls (0),
    m_ipv4 (0),
    m_tcpSocket (0),
    m_reader (0),
    m_routerId (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Schedule (Seconds (0), &LdpProtocol::Initialize, this);
}

LdpProtocol::~LdpProtocol()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
LdpProtocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  for (PeerList::iterator i = m_peers.begin () ; i != m_peers.end (); ++i)
    {
      *i = 0;
    }

  m_peers.clear ();

  for (SocketList::iterator i = m_sockets.begin () ; i != m_sockets.end (); ++i)
    {
      (*i)->Close ();
      *i = 0;
    }

  m_sockets.clear ();

  if (m_tcpSocket != 0)
    {
      m_tcpSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_tcpSocket->Close ();
      m_tcpSocket = 0;
    }
  else
    {
      NS_LOG_WARN("LDP found null socket to close in DoDispose");
    }

  m_node = 0;
  m_ipv4 = 0;
  m_mpls = 0;
  m_reader = 0;

  Object::DoDispose ();
}

void
LdpProtocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = GetObject<Node> ();
      if (node != 0)
        {
          this->SetNode (node);
        }
    }

  if (m_ipv4 == 0)
    {
      Ptr<Ipv4L3Protocol> ipv4 = GetObject<Ipv4L3Protocol> ();
      if (ipv4 != 0)
        {
          this->SetIpv4 (ipv4);
        }
    }

  if (m_mpls == 0)
    {
      Ptr<MplsRoutingProtocol> mpls = GetObject<MplsRoutingProtocol> ();
      if (mpls != 0)
        {
          this->SetMpls (mpls);
        }
    }

  Object::NotifyNewAggregate ();
}

uint32_t
LdpProtocol::GetRouterId (void) const
{
  return m_routerId;
}

void
LdpProtocol::SetKeepAliveTime (uint16_t time)
{
  NS_LOG_FUNCTION (this << time);
  NS_ASSERT (time > 0);
  m_keepAliveTime = time;
}

uint16_t
LdpProtocol::GetKeepAliveTime (void) const
{
  return m_keepAliveTime;
}

void
LdpProtocol::SetHelloInterval (uint16_t interval)
{
  NS_LOG_FUNCTION (this << interval);
  NS_ASSERT (interval > 0);
  m_helloInterval = interval;
}

uint16_t
LdpProtocol::GetHelloInterval (void) const
{
  return m_helloInterval;
}

void
LdpProtocol::SetMaxPduLength (uint16_t length)
{
  NS_LOG_FUNCTION (this << length);
  NS_ASSERT (length >= 256 && length <= 4096);
  m_maxPduLength = length;
}

uint16_t
LdpProtocol::GetMaxPduLength (void) const
{
  return m_maxPduLength;
}

Ptr<LdpPeer>
LdpProtocol::GetPeerForDeviceIfIndex (int32_t ifIndex) const
{
  for (PeerList::const_iterator i = m_peers.begin (); i != m_peers.end (); ++i)
    {
      if ((*i)->GetIfIndex () == ifIndex)
        {
          return *i;
        }
    }

  return 0;
}

Ptr<LdpPeer>
LdpProtocol::GetPeerForDevice (Ptr<const NetDevice> device) const
{
  int32_t ifIndex = m_mpls->GetMplsDeviceIndex (device);

  if (ifIndex < 0)
    {
      return 0;
    }

  return GetPeerForDeviceIfIndex (ifIndex);
}

Ptr<LdpPeer>
LdpProtocol::GetPeerForInterface (Ptr<const Ipv4Interface> interface) const
{
  return GetPeerForDevice (interface->GetDevice ());
}

Ptr<LdpPeer>
LdpProtocol::GetPeerForAddress (const Address &address) const
{
  Ptr<LdpPeer> peer = FindPeer (address);

  if (peer == 0)
    {
      return 0;
    }

  if (peer->GetState () != LdpPeer::OPERATIONAL_STATE)
    {
      return 0;
    }

  return peer;
}

void
LdpProtocol::InsertExtension (Ptr<LdpExtension> extension)
{
  m_extensions.push_back (extension);
}

void
LdpProtocol::RemoveExtension (Ptr<LdpExtension> extension)
{
  m_extensions.remove (extension);
}

Ptr<MplsLibEntry>
LdpProtocol::Bind (uint32_t inLabel, int32_t inIfIndex, uint32_t outLabel, int32_t outIfIndex)
{
  Ptr<MplsLibEntry> entry = Create<MplsLibEntry> ();
  entry->SetInIfIndex (inIfIndex);
  entry->SetInLabel (inLabel);
  entry->SetOutIfIndex (outIfIndex);
  entry->SetOutLabel (outLabel);

  if (outLabel == uint32_t(-1))
    {
      entry->SetOperation (MplsLibEntry::POP);
    }
  else
    {
      entry->SetOperation (MplsLibEntry::SWAP);
    }

  m_mpls->AddLibEntry (entry);

  return entry;
}

Ptr<MplsLibEntry>
LdpProtocol::Bind (const ForwardingEquivalenceClass &fec, uint32_t outLabel, int32_t outIfIndex)
{
  Ptr<MplsLibEntry> entry = Create<MplsLibEntry> ();
  entry->SetFec (fec);
  entry->SetOutIfIndex (outIfIndex);
  entry->SetOperation (MplsLibEntry::PUSH);
  entry->SetOutLabel (outLabel);

  m_mpls->AddLibEntry (entry);

  return entry;
}

void
LdpProtocol::Unbind (Ptr<MplsLibEntry> entry)
{
  m_mpls->RemoveLibEntry (entry);
}

void
LdpProtocol::Initialize ()
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (m_node != 0, "LdpProtocol::Initialize (): Bad node");
  NS_ASSERT_MSG (m_mpls != 0, "LdpProtocol::Initialize (): MplsRoutingProtocol is not available");
  NS_ASSERT_MSG (m_ipv4 != 0, "LdpProtocol::Initialize (): Ipv4 is not available");

  CancelEvents ();

  m_tcpSocket = Socket::CreateSocket (m_node, TcpSocketFactory::GetTypeId ());
  m_tcpSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), LDP_PORT));
  m_tcpSocket->SetAcceptCallback (MakeCallback (&LdpProtocol::HandleConnectionRequest, this),
                                    MakeCallback (&LdpProtocol::HandleConnectionAccept, this));
  m_tcpSocket->Listen ();

  Ipv4Address loopback = "127.0.0.1";
  uint32_t nInterfaces = m_ipv4->GetNInterfaces ();

  for (uint32_t i = 0; i < nInterfaces; ++i)
    {
      Ptr<Ipv4Interface> interface = m_ipv4->GetInterface (i);
      if (m_mpls->GetMplsDeviceIndex (interface->GetDevice ()) < 0)
        {
          continue;
        }

      Ipv4InterfaceAddress iaddr = interface->GetAddress (0);
      Ipv4Address addr = iaddr.GetLocal ();

      if (m_routerId == 0)
        {
          // first available address of router interface
          m_routerId = addr.Get ();
        }

      NS_LOG_DEBUG (this << ": Bind socket to address " << addr);
      Ptr<Socket> socket = Socket::CreateSocket (m_node, UdpSocketFactory::GetTypeId ());
      socket->Bind (InetSocketAddress (addr, LDP_PORT));
      socket->SetRecvCallback (MakeCallback (&LdpProtocol::HandleHelloRead, this));
      m_sockets.push_back (socket);
    }

  m_reader = Create<PduReader> ();

  m_helloEvent = Simulator::Schedule (Seconds (0.0), &LdpProtocol::SendHelloMessage, this);
}

void
LdpProtocol::CancelEvents (void)
{
  NS_LOG_FUNCTION (this);

  Simulator::Cancel (m_helloEvent);
}

void
LdpProtocol::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (node);
  m_node = node;
}

void
LdpProtocol::SetMpls (Ptr<MplsRoutingProtocol> mpls)
{
  NS_LOG_FUNCTION (mpls);
  m_mpls = mpls;
}

void
LdpProtocol::SetIpv4 (Ptr<Ipv4L3Protocol> ipv4)
{
  NS_LOG_FUNCTION (ipv4);
  m_ipv4 = ipv4;
}

Ptr<LdpPeer>
LdpProtocol::FindPeer (const Address &address) const
{
  for (PeerList::const_iterator i = m_peers.begin (); i != m_peers.end (); ++i)
    {
      if ((*i)->GetAddress () == address)
        {
          return *i;
        }
    }
  return 0;
}

bool
LdpProtocol::HandleConnectionRequest (Ptr<Socket> socket, const Address& from) const
{
  NS_LOG_FUNCTION (this << socket << from);
  if (!InetSocketAddress::IsMatchingType (from))
    {
      return false;
    }

  InetSocketAddress addr = InetSocketAddress::ConvertFrom (from);
  return FindPeer (addr.GetIpv4 ()) != 0;
}

void
LdpProtocol::HandleConnectionAccept (Ptr<Socket> socket, const Address& from) const
{
  NS_LOG_FUNCTION (this << socket << from);

  InetSocketAddress addr = InetSocketAddress::ConvertFrom (from);
  Ptr<LdpPeer> peer = FindPeer (addr.GetIpv4 ());

  NS_ASSERT_MSG (peer != 0, "LdpProtocol:HandleConnectionAccept (): bad peer");
  NS_ASSERT_MSG (!peer->IsActiveRole (), "LdpProtocol:HandleConnectionAccept (): bad peer, invalid peer role");

  peer->Initialize (socket, false);
}

void
LdpProtocol::HandlePeerMessage (Ptr<LdpPeer> peer, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << peer << message);

  uint32_t errno = 0;

  for (ExtensionList::const_iterator i = m_extensions.begin (); i != m_extensions.end (); ++i)
  {
    if ((*i)->ReceiveMessage (peer, message, errno))
      {
        break;
      }
  }
}

void
LdpProtocol::HandlePeerClose (Ptr<const LdpPeer> peer)
{
  NS_LOG_FUNCTION (this << peer);

  for (PeerList::iterator i = m_peers.begin (); i!= m_peers.end (); ++i)
    {
      if (peer == (*i))
        {
          m_peers.erase (i);
          break;
        }
    }
}

Ptr<Message>
LdpProtocol::CreateHelloMessage () const
{
  Ptr<CommonHelloParamsTLV> params = Create<CommonHelloParamsTLV> ();
  params->SetHoldTime (m_helloInterval * 3 / 2); // request hello adjacency hold time = 3/2 * helloInterval
  Ptr<Message> message = Create<Message> (HELLO_MESSAGE);
  message->AddValue (params);
  return message;
}

void
LdpProtocol::SendHelloMessage (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<PduWriter> writer = Create<PduWriter> ();
  writer->SetLdpId (GetRouterId ());
  writer->AddMessage (CreateHelloMessage ());
  Ptr<Packet> packet = writer->Write ();

  for (SocketList::const_iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      Address sockName;
      (*i)->GetSockName (sockName);
      InetSocketAddress sockAddr = InetSocketAddress::ConvertFrom (sockName);
      Ipv4Address addr = sockAddr.GetIpv4 ();
      uint32_t nInterfaces = m_ipv4->GetNInterfaces ();
      for (uint32_t j = 0; j < nInterfaces; ++j)
        {
          Ptr<Ipv4Interface> interface = m_ipv4->GetInterface (j);
          Ipv4InterfaceAddress ifAddr= interface->GetAddress (0);
          if (ifAddr.GetLocal () == addr)
            {
              (*i)->SendTo (packet, 0, InetSocketAddress (ifAddr.GetBroadcast (), LDP_PORT));
              break;
            }
        }
    }

  m_helloEvent = Simulator::Schedule (Seconds (m_helloInterval), &LdpProtocol::SendHelloMessage, this);
}

void
LdpProtocol::HandleHelloRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Address from;
  uint8_t buffer[256];
  while (packet = socket->RecvFrom (from))
    {
      uint32_t packetSize = packet->GetSize();
      if (packetSize < 26 || packetSize > 256)
        {
          continue;
        }

      if (!InetSocketAddress::IsMatchingType (from))
        {
          continue;
        }

      InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
      if (address.GetPort () != LDP_PORT)
        {
          continue;
        }

      Address sockName;
      socket->GetSockName (sockName);
      InetSocketAddress sockAddr = InetSocketAddress::ConvertFrom (sockName);
      Ipv4Address addr = sockAddr.GetIpv4 ();
      uint32_t ifIndex = -1;
      uint32_t nInterfaces = m_ipv4->GetNInterfaces ();

      for (uint32_t i = 0; i < nInterfaces; ++i)
        {
          Ptr<Ipv4Interface> interface = m_ipv4->GetInterface (i);
          Ipv4InterfaceAddress ifAddr = interface->GetAddress (0);
          if (ifAddr.GetLocal () == addr)
            {
              ifIndex = interface->GetDevice ()->GetIfIndex ();
              break;
            }
        }

      packet->CopyData (buffer, packetSize);
      m_reader->Reset ();
      m_reader->Feed (buffer, packetSize);

      Ptr<const Message> message;
      while (message = m_reader->GetNextMessage ())
        {
          if (HandleHelloMessage (ifIndex, address.GetIpv4 (), m_reader->GetLdpId (), message))
            {
              Ptr<PduWriter> writer = Create<PduWriter> ();
              writer->SetLdpId (GetRouterId ());
              writer->AddMessage (CreateHelloMessage ());
              socket->SendTo (writer->Write (), 0, address);
            }
        }
    }
}

bool
LdpProtocol::HandleHelloMessage (uint32_t ifIndex, const Ipv4Address &address, uint32_t routerId, Ptr<const Message> message)
{
  NS_LOG_FUNCTION (this << ifIndex << address << routerId << Simulator::Now ());

  uint32_t localRouterId = GetRouterId ();
  if (routerId == localRouterId)
    {
      // XXX: invalid Hello, just drop message?
      NS_LOG_DEBUG ("LdpProtocol::HandleHelloMessage (): "
                    "Drop Hello message from " << routerId << ". "
                    "Duplication of router id");
      return false;
    }

  Message::Iterator i = message->Begin ();
  Ptr<const CommonHelloParamsTLV> params = DynamicCast<const CommonHelloParamsTLV> (*i++);
  if (params == 0)
    {
      // XXX: invalid Hello, just drop message?
      NS_LOG_DEBUG ("LdpProtocol::HandleHelloMessage (): "
                    "Drop hello message from " << routerId << " "
                    ".Common hello params expected");
      return false;
    }

  Ptr<LdpPeer> peer = FindPeer (address);
  if (peer != 0)
    {
      NS_LOG_DEBUG ("LdpProtocol::HandleHelloMessage (): already in session");
      peer->ResetHoldTimeout ();
      return false;
    }

  peer = Create<LdpPeer> (this);
  peer->SetAddress (address);
  peer->SetRouterId (routerId);
  peer->SetHoldTime (params->GetHoldTime ());
  peer->SetIfIndex (ifIndex);
  peer->SetCloseCallback (MakeCallback (&LdpProtocol::HandlePeerClose, this));
  peer->SetMessageCallback (MakeCallback (&LdpProtocol::HandlePeerMessage, this));

  if (routerId > localRouterId)
    {
      peer->Initialize (ConnectSocketTo (address), true);
    }

  m_peers.push_back (peer);

  return true;
}

Ptr<Socket>
LdpProtocol::ConnectSocketTo (const Ipv4Address &address) const
{
  Ptr<Socket> socket = Socket::CreateSocket (m_node, TcpSocketFactory::GetTypeId ());

  socket->Bind (InetSocketAddress (Ipv4Address::GetAny ()));
  socket->Connect (InetSocketAddress (address, LDP_PORT));

  return socket;
}

} // namespace ldp
} // namespace ns3
