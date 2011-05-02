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

#ifndef LDP_CONSTRAINT_BASED_ROUTING_H
#define LDP_CONSTRAINT_BASED_ROUTING_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/forwarding-equivalence-class.h"

#include "common-cr-tlv.h"
#include "lsp-tunnel.h"
#include "ldp-peer.h"
#include "ldp-protocol.h"

namespace ns3 {
namespace ldp {

class LdpConstraintBasedRouting : public LdpExtension
{
public:
  static const uint16_t LABEL_MAPPING_MESSAGE;
  static const uint16_t LABEL_REQUEST_MESSAGE;
  static const uint16_t LABEL_WITHDRAW_MESSAGE;

  static TypeId GetTypeId (void);

  LdpConstraintBasedRouting ();
  virtual ~LdpConstraintBasedRouting ();

  typedef Callback<void, uint16_t, Ptr<const LspTunnel> > OperationSuccessCallback;
  typedef Callback<void, uint16_t> OperationFailCallback;

  uint16_t CreateLspTunnel (Ptr<const ExplicitRouteTLV> ertlv, OperationSuccessCallback scb, OperationFailCallback fcb);
  void DeleteLspTunnel (uint16_t lspid);
//  void ModifyLspTunnel (uint16_t lspid, OperationSuccessCallback scb, OperationFailCallback fcb);

  Ptr<const LspTunnel> GetLspTunnel (uint16_t lspid) const;

  virtual bool ReceiveMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno);

protected:
  virtual void NotifyNewAggregate (void);
  virtual void DoDispose (void);

private:
  struct Operation
  {
    Ptr<LdpPeer> peer;
    uint16_t lspid;
    OperationSuccessCallback scb;
    OperationFailCallback fcb;
    inline Operation (Ptr<LdpPeer> peer, uint16_t lspid, OperationSuccessCallback scb, OperationFailCallback fcb)
    {
      this->peer = peer;
      this->lspid = lspid;
      this->scb = scb;
      this->fcb = fcb;
    }
  };

  struct Request
  {
    Ptr<LdpPeer> inPeer;
    Ptr<LdpPeer> outPeer;
    uint32_t inMsgId;
    uint32_t outMsgId;

    inline Request (Ptr<LdpPeer> peerOut, uint32_t msgOut, Ptr<LdpPeer> peerIn = 0, uint32_t msgIn = 0)
      : inPeer (peerIn), outPeer (peerOut), inMsgId (msgIn), outMsgId (msgOut)
    {
    }
  };

  typedef std::list<Request> RequestList;
  typedef std::list<Operation> OperationList;
  typedef std::list<std::pair<uint16_t, Ptr<LspTunnel> > > LspTunnelList;

  void SetLdp (Ptr<LdpProtocol> ldp);

  void DeallocateTunnel (uint16_t lspid);
  uint16_t AllocateTunnel (void);

  void HandleLspTunnelCreateComplete (Ptr<LdpPeer> peer, uint32_t label);
  Ptr<Message> CreateLabelRequestMessage (Ptr<const LspIdTLV> lsp, Ptr<const ExplicitRouteTLV> ertlv) const;
  Ptr<Message> CreateLabelMappingMessage (uint32_t label, uint32_t messageId) const;
  Ptr<Message> CreateLabelWithdrawMessage (uint32_t label) const;
  bool HandleLabelRequestMessage (Ptr<LdpPeer> session, Ptr<const Message> message, uint32_t &errno);
  bool HandleLabelMappingMessage (Ptr<LdpPeer> session, Ptr<const Message> message, uint32_t &errno);
  bool HandleLabelWithdrawMessage (Ptr<LdpPeer> session, Ptr<const Message> message, uint32_t &errno);

  Ptr<LdpProtocol> m_ldp;
  RequestList m_requests;
  OperationList m_operations;
  LspTunnelList m_tunnels;
  LspTunnelList::iterator m_tunnelIter;
  uint16_t m_availLspId;
  uint16_t m_unavailLspId;
};

} // namespace ldp
} // namespace ns3

#endif /* LDP_CONSTRAINT_BASED_ROUTING_H */
