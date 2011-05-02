/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#ifndef MPLS_TUNNEL_HELPER_H
#define MPLS_TUNNEL_HELPER_H

#include "ns3/ptr.h"
#include "ns3/mpls-interface.h"

#include "mpls-network-helper-base.h"

namespace ns3 {

class MplsTunnelHelper;

/**
 * \brief Tunnel identifier
 */
class TunnelId
{
public:
  ~TunnelId ();

  class Hash : public std::unary_function<TunnelId, size_t> 
  {
  public:
    size_t operator()(TunnelId const &x) const;
  };

private:
  TunnelId (uint32_t id);

  uint32_t m_id;
  friend class MplsTunnelHelper;
};


class Lsp;

class LspNode 
{
public:
  LspNode (const char* addr);
  LspNode (const Ipv4Address &addr);
  LspNode (const Ptr<Node> &node);
  ~LspNode ();
  
  Lsp operator+ (const LspNode &x) const;
  
  const Ipv4Address& GetAddress (void) const;
  const Ptr<Node>& GetNode (void) const;

private:
  Ipv4Address m_address;
  Ptr<Node> m_node;
};


class Lsp
{
public:
  Lsp ();
  Lsp (const Lsp &lsp);
  ~Lsp ();
  
  Lsp& Add (const LspNode &node);
  Lsp& operator= (const Lsp &lsp);
  Lsp operator+ (const LspNode &node) const;
  Lsp operator+ (const Lsp &lsp) const;
  
  typedef std::list<LspNode>::const_iterator Iterator;

  Iterator Begin (void) const;
  Iterator End (void) const;  

private:
  std::list<LspNode> m_nodes;
};


/**
 * \brief 
 */
class MplsTunnelHelper : public MplsNetworkHelperBase
{
public:
  /**
   * @brief Create a new tunnel helper
   */
  MplsTunnelHelper(void);
  /**
   * @brief Destroy object
   */
  virtual ~MplsTunnelHelper(void);

  MplsTunnelHelper (const MplsTunnelHelper &o);
  MplsTunnelHelper& operator= (const MplsTunnelHelper &o);
  
  TunnelId CreateTunnel (const Lsp &lsp);
  void DestroyTunnel (const TunnelId &tunnel);
    
private:
/*  class TunnelMap : public SimpleRefCount<TunnelMap>
  {
  public:
    typedef sgi::hash_map<TunnelId, TunnelId::Hash> HashMap;
    typedef sgi::hash_map<TunnelId, TunnelId::Hash>::iterator Iterator;
    HashMap m_tunnels;
  };
  
  Ptr<Interface> FindInterfaceForAddress (const Ipv4Address &addr);
  Ptr<TunnelMap> m_tunnels;*/
};

} // namespace ns3

#endif /* MPLS_TUNNEL_HELPER_H */
