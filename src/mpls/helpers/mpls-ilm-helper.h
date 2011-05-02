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

#ifndef MPLS_ILM_HELPER_H
#define MPLS_ILM_HELPER_H

#include "ns3/ptr.h"
#include "ns3/mpls.h"

#include "ns3/mpls-label.h"
#include "ns3/mpls-incoming-label-map.h"
#include "ns3/mpls-ilm-table.h"
#include "ns3/mpls-nhlfe.h"

#include "mpls-node-helper-base.h"

namespace ns3 {

using namespace mpls;

/**
 * \brief Mpls ilm table helper
 */
class MplsIlmHelper : public MplsNodeHelperBase
{
public:
  /**
   * @brief Destroy the object
   */
  virtual ~MplsIlmHelper();  
  /**
   * @brief Remove ILM 
   */
  void RemoveIlm (const Ptr<IncomingLabelMap> &ilm);
  /**
   * @brief Clear ILM table
   */
  void ClearIlmTable ();
  /**
   * @brief Add ILM
   * @param interface Incoming mpls interface
   * @param label Incoming label
   * @param nhlfe NHLFE
   * @return ILM
   */
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10);                    
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const Nhlfe &nhlfe12);

  /**
   * @brief Add ILM
   * @param interface Incoming mpls interface
   * @param label Incoming label
   * @param nhlfe NHLFE
   * @param policy NhlfeSelectionPolicyHelper
   * @return ILM 
   */
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe, 
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, 
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, 
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, 
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10,
                    const NhlfeSelectionPolicyHelper& policy);                    
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (uint32_t interface, Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const Nhlfe &nhlfe12,
                    const NhlfeSelectionPolicyHelper& policy);

  /**
   * @brief Add ILM 
   * @param label
   * @param nhlfe
   * @return ILM
   */
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10);                    
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const Nhlfe &nhlfe12);
  /**
   * @brief Add ILM
   * @param label
   * @param nhlfe
   * @param NhlfeSelectionPolicyHelper
   * @return ILM
   */
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe, 
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3,
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, 
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, 
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
                    const NhlfeSelectionPolicyHelper &policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10,
                    const NhlfeSelectionPolicyHelper& policy);                    
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const NhlfeSelectionPolicyHelper& policy);
  Ptr<IncomingLabelMap> AddIlm (Label label, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, 
                    const Nhlfe &nhlfe3, const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, 
                    const Nhlfe &nhlfe6, const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, 
                    const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const Nhlfe &nhlfe12,
                    const NhlfeSelectionPolicyHelper& policy);
};

} // namespace ns3

#endif /* MPLS_ILM_HELPER_H */
