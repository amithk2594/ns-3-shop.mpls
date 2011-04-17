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

#ifndef MPLS_TUNNEL_NODE_H
#define MPLS_TUNNEL_NODE_H

#include <list>

#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

#include "ns3/mpls.h"

namespace ns3 {

class TunnelNodeList;

/**
 * \brief 
 */
class TunnelNode
{
public:
  /**
   * @brief Create a new TunnelNode
   */
  TunnelNode(const Ipv4Address &address);
  /**
   * @brief Create a new TunnelNode
   */
  TunnelNode(const char* address);  
  /**
   * @brief Destroy the MplsNetworkConfigurator
   */
  virtual ~TunnelNode();

  TunnelNode (const TunnelNode &o);
  TunnelNode& operator= (const TunnelNode &o);
  TunnelNodeList operator>> (const TunnelNode &o);
  
  const Ipv4Address& GetAddress (void) const;
  
private:
  Ipv4Address m_address;
  
  friend class TunnelNodeList;
  friend std::ostream& operator<< (std::ostream& os, const TunnelNode &node);
};


class TunnelNodeList
{
public:
  /**
   * @brief Create empty tunnel node list
   */
  TunnelNodeList();
  /**
   * @brief Destroy object
   */
  ~TunnelNodeList();

  TunnelNodeList (const TunnelNodeList &o);
  TunnelNodeList& operator= (const TunnelNodeList &o);
  
  TunnelNodeList& Add (const TunnelNode &o);
  TunnelNodeList& Add (const TunnelNodeList &o);  
  
  TunnelNodeList operator>> (const TunnelNodeList &o);
  TunnelNodeList operator>> (const TunnelNode &o);
  
  typedef std::list<TunnelNode>::const_iterator Iterator;
  
  Iterator Begin (void) const;
  Iterator End (void) const;
  
  uint32_t GetN (void) const;
  
private:
  std::list<TunnelNode> m_nodes;
  
  friend std::ostream& operator<< (std::ostream& os, const TunnelNodeList &nodes);
};

std::ostream& operator<< (std::ostream& os, const TunnelNode &node);
std::ostream& operator<< (std::ostream& os, const TunnelNodeList &nodes);

} // namespace ns3

#endif /* MPLS_TUNNEL_NODE_H */
