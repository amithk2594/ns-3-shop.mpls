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

#ifndef MPLS_INTERFACE_HELPER_H
#define MPLS_INTERFACE_HELPER_H

#include <ostream>
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/ptr.h"


namespace ns3 {

class Node;

/**
 * \brief Mpls interface helper
 */
class MplsInterfaceHelper
{
public:
  /**
   * @brief Create a new MplsInterfaceHelper
   */
  MplsInterfaceHelper(void);

  /**
   * @brief Destroy the MplsInterfaceHelper
   */
  virtual ~MplsInterfaceHelper(void);

  /**
   * @brief Enable MPLS interfaces auto install onto the provided node.
   * 
   * @param nodeName The name of the node on which to install MPLS stack.
   */
  void EnableInterfaceAutoInstall (std::string nodeName) const;

  /**
   * @brief Enable MPLS interfaces auto install onto the provided node.
   * 
   * @param node
   */
  void EnableInterfaceAutoInstall (Ptr<Node> node) const;

  /**
   * @brief Enable MPLS interfaces auto install for each node in the input container.
   * 
   * @param c NodeContainer that holds the set of nodes
   */
  void EnableInterfaceAutoInstall (NodeContainer c) const;

  /**
   * @brief Enable MPLS interfaces auto install for all nodes in the simulation
   * with mpls installed
   */
  void EnableInterfaceAutoInstallAll (void) const;
  
  /**
   * @brief Disable MPLS interfaces auto install onto the provided node.
   * 
   * @param nodeName The name of the node.
   */
  void DisableInterfaceAutoInstall (std::string nodeName) const;

  /**
   * @brief Disable MPLS interfaces auto install onto the provided node.
   * 
   * @param node
   */
  void DisableInterfaceAutoInstall (Ptr<Node> node) const;

  /**
   * @brief Disable MPLS interfaces auto install for each node in the input container.
   * 
   * @param c NodeContainer that holds the set of nodes
   */
  void DisableInterfaceAutoInstall (NodeContainer c) const;

  /**
   * @brief Disable MPLS interfaces auto install for all nodes in the simulation 
   * with mpls installed
   */
  void DisableInterfaceAutoInstallAll (void) const;

  /**
   * @brief Show MPLS interfaces
   * 
   * @param node
   */
  void ShowInterfaces (Ptr<Node> node, std::ostream& os = std::cout) const;

  /**
   * @brief Show MPLS interfaces
   * 
   * @param nodeName The name of the node.
   */
  void ShowInterfaces (std::string nodeName, std::ostream& os = std::cout) const;
  
  /**
   * @brief Show MPLS interfaces
   * 
   * @param c NodeContainer that holds the set of nodes
   */
  void ShowInterfaces (NodeContainer c, std::ostream& os = std::cout) const;

  /**
   * @brief Show MPLS interfaces
   * with mpls installed
   */
  void ShowAllInterfaces (std::ostream& os = std::cout) const;
};

} // namespace ns3

#endif /* MPLS_INTERFACE_HELPER_H */
