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

#ifndef MPLS_SWITCH_H
#define MPLS_SWITCH_H

#include "ns3/ptr.h"
#include "ns3/mpls-node.h"
#include "ns3/mpls.h"
#include "mpls-ftn-helper.h"
#include "mpls-ilm-helper.h"

namespace ns3 {

/**
 * \brief Mpls switch configurator
 */
class MplsSwitch : public MplsFtnHelper, public MplsIlmHelper
{
public:
  /**
   * @brief Create a new MplsSwitch object
   */
  MplsSwitch (const Ptr<Node> &node);
  /**
   * @brief Create a new MplsSwitch object
   */
  MplsSwitch (const std::string &name);
  /**
   * @brief Destroy the MplsSwitch
   */
  virtual ~MplsSwitch ();

  MplsSwitch (const MplsSwitch &o);
  MplsSwitch& operator= (const MplsSwitch &o);
  
  /**
   * @brief Add mpls interface for specified device
   * @param devIfIndex Device index
   */
  Ptr<mpls::Interface> AddInterface (uint32_t devIfIndex);
  /**
   * @brief Returns interface 
   */
  Ptr<mpls::Interface> GetInterface (uint32_t i);
  /**
   * @brief Return true if interface is enabled
   */
  bool IsUp (uint32_t i) const;
  /**
   * @brief Enable interface
   */
  void SetUp (uint32_t i);
  /**
   * @brief Disable interface
   */
  void SetDown (uint32_t i);
  /**
   * @brief Set NHLFE selection policy helper
   */
  void SetSelectionPolicy(const NhlfeSelectionPolicyHelper& policy);

  virtual const Ptr<Mpls>& GetMpls (void) const;
  virtual const Ptr<MplsNode>& GetNode (void) const;
  virtual const NhlfeSelectionPolicyHelper& GetSelectionPolicy (void) const;
  
private:
  Ptr<MplsNode> m_node;
  Ptr<Mpls> m_mpls;  
  NhlfeSelectionPolicyHelper m_nhlfeSelectionPolicy;
};

} // namespace ns3

#endif /* MPLS_SWITCH_HELPER_H */
