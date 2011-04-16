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

#include "protocol-data-unit.h"
#include "fec-tlv.h"
#include "ldp-status-codes.h"
#include "common-tlv.h"
#include "ldp-constraint-based-routing.h"

NS_LOG_COMPONENT_DEFINE ("LdpConstraintBasedRouting");

namespace ns3 {
namespace ldp {

NS_OBJECT_ENSURE_REGISTERED (LdpConstraintBasedRouting);

const uint16_t LdpConstraintBasedRouting::LABEL_MAPPING_MESSAGE = 0x0400;
const uint16_t LdpConstraintBasedRouting::LABEL_REQUEST_MESSAGE = 0x0401;
const uint16_t LdpConstraintBasedRouting::LABEL_WITHDRAW_MESSAGE = 0x0402;

TypeId
LdpConstraintBasedRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ldp::LdpConstraintBasedRouting")
    .SetParent<Object> ()
    .AddConstructor <LdpConstraintBasedRouting> ()
    ;
  return tid;
}

LdpConstraintBasedRouting::LdpConstraintBasedRouting ()
  : m_ldp (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_availLspId = 0;
  m_unavailLspId = 0xffff;
  m_tunnelIter = m_tunnels.begin ();
}

LdpConstraintBasedRouting::~LdpConstraintBasedRouting ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
LdpConstraintBasedRouting::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  for (RequestList::iterator i = m_requests.begin (); i != m_requests.end (); ++i)
    {
      (*i).inPeer = 0;
      (*i).outPeer = 0;
    }
  m_requests.clear ();

  for (LspTunnelList::iterator i = m_tunnels.begin (); i != m_tunnels.end (); ++i)
    {
      (*i).second = 0;
    }
  m_tunnels.clear ();

  m_ldp->RemoveExtension (this);
  m_ldp = 0;

  LdpExtension::DoDispose ();
}

void
LdpConstraintBasedRouting::NotifyNewAggregate ()
{
  if (m_ldp == 0)
    {
      Ptr<LdpProtocol> ldp = GetObject<LdpProtocol> ();
      if (ldp != 0)
        {
          this->SetLdp (ldp);
        }
    }

  LdpExtension::NotifyNewAggregate ();
}

void
LdpConstraintBasedRouting::SetLdp (Ptr<LdpProtocol> ldp)
{
  NS_LOG_FUNCTION (this << ldp);

  m_ldp = ldp;
  ldp->InsertExtension (this);
}

void
LdpConstraintBasedRouting::DeallocateTunnel (uint16_t lspid)
{
  for (LspTunnelList::iterator i = m_tunnels.begin (); i != m_tunnels.end (); ++i)
    {
      if ((*i).first == lspid)
        {
          (*i).second = 0;
          m_tunnels.erase (i);
          return;
        }
    }
}

uint16_t
LdpConstraintBasedRouting::AllocateTunnel ()
{
  if (m_availLspId < m_unavailLspId)
    {
      uint16_t i = m_availLspId++;
      m_tunnels.insert (m_tunnelIter, std::make_pair(i, Ptr<LspTunnel> (0)));
      return i;
    }

  NS_ASSERT_MSG (m_tunnels.size () < 0xffff, "LdpConstraintBasedRouting::AllocateLspId (): "
                                              "LSP tunnels limit exceeded");

  m_availLspId = 0;
  m_tunnelIter = m_tunnels.begin ();
  while (m_tunnelIter != m_tunnels.end ())
    {
      m_unavailLspId = (*m_tunnelIter).first;
      if (m_availLspId < m_unavailLspId)
        {
          return AllocateTunnel ();
        }
      m_availLspId = m_unavailLspId + 1;
      m_tunnelIter++;
    }

  m_unavailLspId = 0xffff;
  return AllocateTunnel ();
}

uint16_t
LdpConstraintBasedRouting::CreateLspTunnel (Ptr<const ExplicitRouteTLV> ertlv, OperationSuccessCallback scb,
  OperationFailCallback fcb)
{
  NS_LOG_FUNCTION (this);

  Ptr<const Ipv4ExplicitRouteHopTLV> hoptlv = DynamicCast<const Ipv4ExplicitRouteHopTLV> (ertlv->GetFirstRouteHop ());
  if (hoptlv == 0)
    {
      NS_LOG_DEBUG ("There is no next-hop"); // think
      return -1;
    }

  Ipv4Address nextHop = hoptlv->GetAddress ();
  Ptr<LdpPeer> peer = m_ldp->GetPeerForAddress (nextHop);

  if (peer == 0)
    {
      NS_LOG_DEBUG ("No peers found for next-hop " << nextHop); // think
      return -1;
    }

  NS_LOG_DEBUG ("Send Label Request to next-hop " << nextHop);

  uint16_t lspid = AllocateTunnel ();
  Ptr<Message> message = CreateLabelRequestMessage (Create<LspIdTLV> (true, lspid, m_ldp->GetRouterId ()), ertlv);
  peer->Send (message);

  m_requests.push_back (Request (peer, message->GetMessageId ()));
  m_operations.push_back (Operation (peer, lspid, scb, fcb));
  return lspid;
}

void
LdpConstraintBasedRouting::DeleteLspTunnel (uint16_t lspid)
{
  NS_LOG_FUNCTION (this << lspid);
  LspTunnelList::iterator i = m_tunnels.begin ();

  for (; i != m_tunnels.end (); ++i)
    {
      if ((*i).first == lspid)
        {
          break;
        }
    }

  if (i == m_tunnels.end ())
    {
      NS_LOG_DEBUG ("Invalid Lsp ID " << lspid);
      return;
    }

  Ptr<LspTunnel> tunnel = (*i).second;
  if (tunnel == 0)
    {
      NS_LOG_DEBUG ("Unitialized tunnel " << lspid);
      return;
    }

  Ptr<LdpPeer> peer = m_ldp->GetPeerForDeviceIfIndex (tunnel->GetOutIfIndex ());
  if (peer == 0)
    {
      NS_LOG_DEBUG ("No peers found for device " << tunnel->GetOutIfIndex ());
      return;
    }

  Ptr<Message> message = CreateLabelWithdrawMessage (tunnel->GetOutLabel ());
  peer->Send (message);
  m_tunnels.erase (i);
}

//void
//LdpConstraintBasedRouting::ModifyLspTunnel (uint16_t lspid, OperationSuccessCallback scb, OperationFailCallback fcb)
//{
//}

Ptr<const LspTunnel>
LdpConstraintBasedRouting::GetLspTunnel (uint16_t lspid) const
{
  for (LspTunnelList::const_iterator i = m_tunnels.begin (); i != m_tunnels.end (); ++i)
    {
      if ((*i).first == lspid)
        {
          return (*i).second;
        }
    }

  return 0;
}

bool
LdpConstraintBasedRouting::ReceiveMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno)
{
  switch (message->GetMessageType ())
  {
    case LABEL_REQUEST_MESSAGE:
      return HandleLabelRequestMessage (peer, message, errno);
    case LABEL_MAPPING_MESSAGE:
      return HandleLabelMappingMessage (peer, message, errno);
    case LABEL_WITHDRAW_MESSAGE:
      return HandleLabelWithdrawMessage (peer, message, errno);
  }

  return false;
}

bool
LdpConstraintBasedRouting::HandleLabelRequestMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno)
{
  NS_LOG_FUNCTION (this << peer << message);

  NS_LOG_DEBUG ("Receive label request message from " <<
                Ipv4Address::ConvertFrom (peer->GetAddress ()));

  Message::Iterator i = message->Begin ();
  Ptr<const FecTLV> fec = DynamicCast<const FecTLV> (*i++);
  if (fec == 0)
    {
      NS_LOG_DEBUG ("Drop label request message. CR-LDP FEC expected.");
      return false;
    }

  Ptr<const LspIdTLV> lspid = DynamicCast<const LspIdTLV> (*i++);
  if (lspid == 0)
  {
    NS_LOG_DEBUG ("Drop label request message. LSPID TLV expected.");
    return false;
  }

  Ptr<const ExplicitRouteTLV> ertlv = DynamicCast<const ExplicitRouteTLV> (*i++);
  if (ertlv == 0)
    {
      return false;
    }

  Ptr<const ExplicitRouteHopTLV> hop = ertlv->GetSecondRouteHop ();
  if (hop == 0)
    {
      uint32_t inLabel = peer->BindLabel ();
      Ptr<Message> mappingMessage = CreateLabelMappingMessage (inLabel, message->GetMessageId ());
      peer->Send (mappingMessage);
      return true;
    }

  Ptr<const Ipv4ExplicitRouteHopTLV> nextHop = DynamicCast<const Ipv4ExplicitRouteHopTLV> (hop);

  Ptr<LdpPeer> outPeer = m_ldp->GetPeerForAddress (nextHop->GetAddress ());
  if (outPeer == 0)
    {
      NS_LOG_DEBUG ("Drop label request message. No peers found for next-hop " << nextHop->GetAddress ());
      return false;
    }

  Ptr<ExplicitRouteTLV> nertlv = ertlv->Copy ();
  nertlv->RemoveFirstRouteHop ();

  Ptr<Message> requestMessage = CreateLabelRequestMessage (lspid, nertlv);
  outPeer->Send (requestMessage);
  m_requests.push_back (Request (outPeer, requestMessage->GetMessageId (), peer, message->GetMessageId ()));
  return true;
}

bool
LdpConstraintBasedRouting::HandleLabelMappingMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno)
{
  NS_LOG_FUNCTION (this << peer << message);

  NS_LOG_DEBUG ("Receive label mapping message from " <<
                Ipv4Address::ConvertFrom (peer->GetAddress ()));

  Message::Iterator i = message->Begin ();
  Ptr<const FecTLV> fec = DynamicCast<const FecTLV> (*i++);
  if (fec == 0)
    {
      NS_LOG_DEBUG ("Drop label mapping message. CR-LDP FEC expected.");
      return false;
    }

  Ptr<const GenericLabelTLV> label = DynamicCast<const GenericLabelTLV> (*i++);
  if (label == 0)
    {
      NS_LOG_DEBUG ("Drop label mapping message. Generic Label TLV expected.");
      return false;
    }

  Ptr<const MessageIdTLV> msgid = DynamicCast<const MessageIdTLV> (*i++);
  if (msgid == 0)
    {
      NS_LOG_DEBUG ("Drop label mapping message. Message ID TLV expected.");
      return false;
    }

  RequestList::iterator j = m_requests.begin ();
  for (; j != m_requests.end (); ++j)
    {
      if ((*j).outPeer == peer && (*j).outMsgId == msgid->GetMessageId ())
        {
          break;
        }
    }

  if (j == m_requests.end ())
    {
      NS_LOG_DEBUG ("Drop mapping message. Not for us.");
      return false;
    }

  Ptr<LdpPeer> inPeer = (*j).inPeer;
  if (inPeer != 0)
    {
      uint32_t inLabel = inPeer->BindLabel (label->GetLabel (), peer->GetIfIndex ());
      Ptr<Message> mappingMessage = CreateLabelMappingMessage (inLabel, (*j).inMsgId);
      inPeer->Send (mappingMessage);
    }
  else
    {
      HandleLspTunnelCreateComplete (peer, label->GetLabel ());
    }

  m_requests.erase (j);
  return true;
}

void
LdpConstraintBasedRouting::HandleLspTunnelCreateComplete (Ptr<LdpPeer> peer, uint32_t label)
{
  NS_LOG_FUNCTION (this << peer << label);

  for (OperationList::iterator i = m_operations.begin (); i != m_operations.end (); ++i)
    {
      if ((*i).peer == peer)
        {
          uint16_t lspid = (*i).lspid;
          Ptr<LspTunnel> tunnel = Create<LspTunnel> (lspid);
          tunnel->SetOutLabel (label);
          tunnel->SetOutIfIndex (peer->GetIfIndex ());

          for (LspTunnelList::iterator j = m_tunnels.begin (); j != m_tunnels.end (); ++j)
            {
              if ((*j).first == lspid)
                {
                  (*j).second = tunnel;
                  break;
                }
            }

          if (!(*i).scb.IsNull ())
            {
              (*i).scb (lspid, tunnel);
            }
          m_operations.erase (i);
          return;
        }
    }
  NS_LOG_DEBUG ("No operations found");
}

bool
LdpConstraintBasedRouting::HandleLabelWithdrawMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno)
{
  NS_LOG_FUNCTION (this << peer << message);

  NS_LOG_DEBUG ("Receive label withdraw message from " <<
                Ipv4Address::ConvertFrom (peer->GetAddress ()));

  Message::Iterator i = message->Begin ();
  Ptr<const FecTLV> fec = DynamicCast<const FecTLV> (*i++);
  if (fec == 0)
    {
      NS_LOG_DEBUG ("Drop label withdraw message. CR-LDP FEC expected.");
      return false;
    }

  Ptr<const GenericLabelTLV> label = DynamicCast<const GenericLabelTLV> (*i++);
  if (label == 0)
    {
      NS_LOG_DEBUG ("Drop label withdraw message. Generic Label TLV expected.");
      return false;
    }

  uint32_t outLabel;
  int32_t outIfIndex;
  if (!peer->UnbindLabel (label->GetLabel (), outLabel, outIfIndex))
    {
      NS_LOG_DEBUG ("Drop label withdraw message. Invalid label.");
      return false;
    }

  Ptr<LdpPeer> outPeer = m_ldp->GetPeerForDeviceIfIndex (outIfIndex);
  if (outPeer == 0)
    {
      NS_LOG_DEBUG ("Drop label withdraw message. Outgoing peer not found.");
      return false;
    }

  Ptr<Message> withdrawMessage = CreateLabelWithdrawMessage (outLabel);
  outPeer->Send (withdrawMessage);

  return true;
}

Ptr<Message>
LdpConstraintBasedRouting::CreateLabelRequestMessage (Ptr<const LspIdTLV> lsp, Ptr<const ExplicitRouteTLV> ertlv) const
{
  Ptr<Message> message = Create<Message> (LABEL_REQUEST_MESSAGE);
  message->AddValue (CrLspFecElement::CreateFecTLV ());
  message->AddValue (lsp);
  message->AddValue (ertlv);
  return message;
}

Ptr<Message>
LdpConstraintBasedRouting::CreateLabelMappingMessage (uint32_t label, uint32_t messageId) const
{
  Ptr<Message> message = Create<Message> (LABEL_MAPPING_MESSAGE);
  message->AddValue (CrLspFecElement::CreateFecTLV ());
  message->AddValue (Create<GenericLabelTLV> (label));
  message->AddValue (Create<MessageIdTLV> (messageId));
  return message;
}

Ptr<Message>
LdpConstraintBasedRouting::CreateLabelWithdrawMessage (uint32_t label) const
{
  Ptr<Message> message = Create<Message> (LABEL_WITHDRAW_MESSAGE);
  message->AddValue (CrLspFecElement::CreateFecTLV ());
  message->AddValue (Create<GenericLabelTLV> (label));
  return message;
}

} // namespace ldp
} // namespace ns3
