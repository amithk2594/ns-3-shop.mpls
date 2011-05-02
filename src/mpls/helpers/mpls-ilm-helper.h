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
   * @brief Return ILM table
   */
  Ptr<mpls::IlmTable> GetIlmTable (void) const;
  /**
   * @brief Set new ILM table
   */
  void SetIlmTable (const Ptr<mpls::IlmTable> &table);
  /**
   * @brief Get the ILM by Index
   * @param index
   * @return ILM identified by index
   */
  Ptr<mpls::IncomingLabelMap> GetIlm (const uint32_t index) const;
  /**
   * @brief Remove ILM by index
   * @param index
   */
  void RemoveIlm (const uint32_t index);
  /**
   * @brief Clear ILM table
   */
  void ClearIlmTable ();
  /**
   * @brief Add a new ILM
   * @param interface Incoming mpls interface
   * @param label Incoming label
   * @param nhlfe NHLFE
   * @return ILM index
   */
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10);                    
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const mpls::Nhlfe &nhlfe12);

  /**
   * @brief Add a new ILM
   * @param interface Incoming mpls interface
   * @param label Incoming label
   * @param nhlfe NHLFE
   * @param policy NhlfeSelectionPolicyHelper
   * @return ILM index
   */
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe, 
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, 
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, 
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, 
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
                    const NhlfeSelectionPolicyHelper& policy);                    
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const mpls::Nhlfe &nhlfe12,
                    const NhlfeSelectionPolicyHelper& policy);

  /**
   * @brief Add ILM with a single NHLFE (per-system label space)
   * @param label
   * @param nhlfe
   * @return index of the ILM
   */
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10);                    
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const mpls::Nhlfe &nhlfe12);
  /**
   * @brief Add ILM with a single NHLFE (per-system label space)
   * @param label
   * @param nhlfe
   * @param NhlfeSelectionPolicyHelper
   * @return index of the ILM
   */
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe, 
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3,
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, 
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7,
                    const NhlfeSelectionPolicyHelper &policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
                    const NhlfeSelectionPolicyHelper& policy);                    
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const NhlfeSelectionPolicyHelper& policy);
  uint32_t AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
                    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
                    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
                    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
                    const mpls::Nhlfe &nhlfe12,
                    const NhlfeSelectionPolicyHelper& policy);
};

} // namespace ns3

#endif /* MPLS_ILM_HELPER_H */
