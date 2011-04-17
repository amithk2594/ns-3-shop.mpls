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

#include "mpls-tunnel-node.h"
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
  
  TunnelId CreateTunnel (const TunnelNodeList &nodes);
  void DestroyTunnel (const TunnelId &tunnel);
    
private:
  class TunnelMap : public SimpleRefCount<TunnelMap>
  {
  public:
    typedef sgi::hash_map<TunnelId, TunnelId::Hash> HashMap;
    typedef sgi::hash_map<TunnelId, TunnelId::Hash>::iterator Iterator;
    HashMap m_tunnels;
  };
  
  Ptr<Interface> FindInterfaceForAddress (const Ipv4Address &addr);
  Ptr<TunnelMap> m_tunnels;
};

} // namespace ns3

#endif /* MPLS_TUNNEL_HELPER_H */
