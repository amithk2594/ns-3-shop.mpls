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

#ifndef TE_SERVER_H
#define TE_SERVER_H

#include <list>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"

#include "identifier-list.h"
#include "sequence-list.h"

namespace ns3 {
namespace ldp {

class TeServer : public Object
{
public:
  static const uint16_t TE_PORT;
  static const uint16_t TUNNEL_CREATE_MESSAGE;
  static const uint16_t TUNNEL_DELETE_MESSAGE;
  static const uint16_t NOTIFICATION_MESSAGE;

  static TypeId GetTypeId (void);

  TeServer ();
  virtual ~TeServer ();

  typedef Callback<void, uint32_t> OperationCallback;

  void SetOperationSuccessCallback (OperationCallback cb);
  void SetOperationFailCallback (OperationCallback cb);

  uint32_t CreateTrunk (const std::vector<Ipv4Address> &pathTo, const std::vector<Ipv4Address> &pathFrom);

  void DeleteTrunk (uint32_t trunkId);

protected:
  virtual void NotifyNewAggregate (void);
  virtual void DoDispose (void);

private:
  class Trunk : public SimpleRefCount<Trunk>
  {
  public:
    uint32_t    id;
    Ptr<Socket> sock1;
    uint16_t    lsp1;
    Ptr<Socket> sock2;
    uint16_t    lsp2;
    inline Trunk (void)
      : id (-1), sock1 (0), lsp1 (-1), sock2 (0), lsp2 (-1)
    {}
  };

  class Request : public SimpleRefCount<Request>
  {
  public:
    Ptr<Socket> socket;
    Ptr<Trunk>  trunk;
    inline Request (Ptr<Socket> s, Ptr<Trunk> t)
      : socket (s), trunk (t)
    {}
  };

  typedef std::list<std::pair<Ipv4Address, Ptr<Socket> > > SocketList;
  typedef SequenceList<uint32_t, Ptr<Request> > RequestList;
  typedef IdentifierList<uint32_t, Ptr<Trunk> > TrunkList;

  void Initialize (void);
  bool HandleConnectionRequest (Ptr<Socket> socket, const Address& from);
  void HandleConnectionAccept (Ptr<Socket> socket, const Address& from);
  void HandleSocketRead (Ptr<Socket> socket);
  void HandleMessage (Ptr<Socket> socket, uint32_t ldpid, Ptr<const Message> message);
  void HandleError (Ptr<Socket> socket, uint32_t ldpid, uint32_t errno);
  void HandleNotificationMessage (Ptr<Socket> socket, uint32_t ldpid, Ptr<const Message> message);
  void SendTunnelCreateMessage (const std::vector<Ipv4Address> &path, Ptr<TeServer::Trunk> trunk);
  void SendTunnelDeleteMessage (Ptr<Socket> socket, uint32_t lsp);
  Ptr<Socket> GetSocket (const Ipv4Address &address);

  Ptr<Node> m_node;
  Ptr<Socket> m_socket;
  Ptr<PduReader> m_reader;
  SocketList m_sockets;
  TrunkList m_trunks;
  RequestList m_requests;
  OperationCallback m_scb;
  OperationCallback m_fcb;

};

} // namespace ldp
} // namespace ns3

#endif /* TE_SERVER_H */
