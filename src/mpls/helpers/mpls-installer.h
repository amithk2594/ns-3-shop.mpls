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

#ifndef MPLS_INSTALLER_H
#define MPLS_INSTALLER_H

#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/packet.h"
#include "ns3/mpls.h"
#include "ns3/mpls-ipv4-protocol.h"

#include "mpls-enum-helper.h"
#include "mpls-interface-helper.h"

namespace ns3 {

class Node;
class Ipv4RoutingHelper;

/**
 * \brief aggregate MPLS/IP/TCP/UDP functionality to existing Nodes.
 */
class MplsInstaller: public MplsInterfaceHelper
{
public:
  /**
   * @brief Create a new MplsInstaller
   */
  MplsInstaller(void);

  /**
   * @brief Destroy the MplsInstaller
   */
  virtual ~MplsInstaller(void);
  MplsInstaller (const MplsInstaller &);
  MplsInstaller &operator = (const MplsInstaller &o);

  /**
   * @brief Return helper internal state to that of a newly constructed one
   */
  void Reset (void);

  /**
   * @param routing a new routing helper
   *
   * Set the routing helper to use during Install.
   */
  void SetRoutingHelper (const Ipv4RoutingHelper &routing);

  /**
   * @brief set the TCP stack which will not need any other parameter.  
   *
   * @param tid the type id, typically it is set to  "ns3::TcpL4Protocol"
   */
  void SetTcp(std::string tid);

  /**
   * @brief Aggregate implementations of MPLS, IPv4, TCP, UDP and ARP onto the provided node.
   * 
   * @param node node, node name or node container
   */
  template <class T>
  void Install (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInstaller::InstallInternal, this));
  }

private:

  void Initialize (void);
  ObjectFactory m_tcpFactory;
  const Ipv4RoutingHelper *m_routing;
  void InstallInternal (Ptr<Node> node) const;
    
  static void CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId);
  static void Cleanup (void);
};

} // namespace ns3

#endif /* MPLS_INSTALLER_H */
