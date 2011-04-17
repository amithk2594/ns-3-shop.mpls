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
#include "ns3/callback.h"
#include "ns3/mpls.h"
#include "ns3/mpls-ipv4-protocol.h"

#include "mpls-enum-helper.h"
#include "mpls-network-helper-base.h"

namespace ns3 {

class Node;
class Ipv4RoutingHelper;

/**
 * \brief Aggregate MPLS/IP/TCP/UDP functionality to existing Nodes.
 */
class MplsInstaller : public MplsNetworkHelperBase
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
   * @param routing A new routing helper
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
  void Install (T node)
  {
    ForEachNode (node, MakeCallback (&MplsInstaller::InstallInternal, this));
  }
  
  /**
   * @brief Enable MPLS interfaces auto install onto the provided node.
   *
   * @param node node name, node or container
   */
  template <class T>
  void EnableInterfaceAutoInstall (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInstaller::EnableInterfaceAutoInstallInternal, this));
  }

  /**
   * @brief Disable MPLS interfaces auto install onto the provided node.
   *
   * @param node node name, node or container
   */
  template <class T>
  void DisableInterfaceAutoInstall (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInstaller::DisableInterfaceAutoInstallInternal, this));
  }

  /**
   * @brief Returns all nodes with MPLS installed
   */
  virtual const NodeContainer& GetNetworkNodes (void) const;

private:
  void Initialize (void);
  void InstallInternal (Ptr<Node> node);
  void EnableInterfaceAutoInstallInternal (Ptr<Node> node) const;
  void DisableInterfaceAutoInstallInternal (Ptr<Node> node) const;

  static void CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId);
  
  ObjectFactory m_tcpFactory;
  const Ipv4RoutingHelper *m_routing;
  
  NodeContainer m_networkNodes;
};

} // namespace ns3

#endif /* MPLS_INSTALLER_H */
