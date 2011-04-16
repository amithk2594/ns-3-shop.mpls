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

#ifndef TRUNK_UTILIZATION_H
#define TRUNK_UTILIZATION_H

#include <list>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/ldp-protocol.h"
#include "ns3/ldp-constraint-based-routing.h"

#include "ldp-protocol.h"

namespace ns3 {
namespace ldp {

class TrunkUtilization : public Object
{
public:
  static const uint16_t TE_PORT;
  static const uint16_t TUNNEL_CREATE_MESSAGE;
  static const uint16_t TUNNEL_DELETE_MESSAGE;
  static const uint16_t NOTIFICATION_MESSAGE;
  static TypeId GetTypeId (void);

  TrunkUtilization ();
  virtual ~TrunkUtilization ();

protected:
  virtual void NotifyNewAggregate (void);
  virtual void DoDispose (void);

private:
  struct Request
  {
    uint16_t lspid;
    uint32_t msgid;
    Ipv4Address address;
    inline Request (uint16_t lid, uint32_t mid, Ipv4Address addr)
      : lspid (lid), msgid (mid), address (addr)
    {
    }
  };

  typedef std::list<Request > RequestList;
  void Initialize (void);
  void HandleSocketRead (Ptr<Socket> socket);
  void HandleSocketClose (Ptr<Socket> socket);
  void HandleMessage (uint32_t ldpid, Ptr<const Message> message);
  void HandleError (uint32_t ldpid, uint32_t errno);
  void HandleTunnelCreateMessage (Ptr<const Message> message);
  void CreateTunnelSuccess (uint16_t lspid, Ptr<const LspTunnel> tunnel);
  void CreateTunnelFail (uint16_t lspid);
  void HandleTunnelDeleteMessage (Ptr<const Message> message);

  Ptr<Node> m_node;
  Ptr<LdpProtocol> m_ldp;
  Ptr<LdpConstraintBasedRouting> m_crldp;
  Ptr<Socket> m_socket;
  Ptr<PduReader> m_reader;
  Ipv4Address m_address;
  RequestList m_requests;
};

} // namespace ldp
} // namespace ns3

#endif /* TRUNK_UTILIZATION_H */
