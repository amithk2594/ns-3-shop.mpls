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

#ifndef MPLS_FTN_HELPER_H
#define MPLS_FTN_HELPER_H

#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/mpls.h"
#include "ns3/mpls-label.h"
#include "ns3/mpls-ftn-table.h"
#include "ns3/mpls-fec-to-nhlfe.h"
#include "ns3/mpls-fec.h"
#include "ns3/mpls-nhlfe.h"

#include "mpls-node-helper-base.h"

namespace ns3 {

/**
 * \brief Mpls ftn table helper
 */
class MplsFtnHelper : public MplsNodeHelperBase
{
public:
  virtual ~MplsFtnHelper();
  /**
   * @brief Return FTN table
   */
  Ptr<mpls::FtnTable> GetFtnTable (void) const;
  /**
   * @brief Set new FTN table
   */
  void SetFtnTable (const Ptr<mpls::FtnTable> &table);
  /**
   * @brief Get the FTN by Index
   * @param index
   * @return FTN identified by index
   */
  Ptr<mpls::FecToNhlfe> GetFtn (const uint32_t index) const;
  /**
   * @brief Remove FTN by index
   * @param index
   */
  void RemoveFtn (const uint32_t index);
  /**
   * @brief Clear FTN table
   */
  void ClearFtnTable ();
  /**
   * @brief Add a new FTN
   * @param fec FEC
   * @param nhlfe NHLFE
   * @return FTN index
   */
  template<class T>
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4);
  template<class T>                    
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5);
  template<class T>                    
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6);
  /**
   * @brief Add a new FTN
   * @param fec FEC
   * @param nhlfe NHLFE
   * @param Nhlfe selection policy
   * @return FTN index
   */
  template<class T>
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  uint32_t AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
                    const NhlfeSelectionPolicyHelper& policy);

};

template<class T>
uint32_t 
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe)
{
  return AddFtn (fec, nhlfe, GetSelectionPolicy ());
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2)
{
  return AddFtn (fec, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                  const mpls::Nhlfe &nhlfe4)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

template<class T>                    
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                  const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

template<class T>                    
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
                  const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}


template<class T>
uint32_t 
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe, policy.CreatePolicy ());
  return GetFtnTable ()->AddFtn (ftn);
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe1, policy.CreatePolicy ());
  ftn->AddNhlfe (nhlfe2);
  return GetFtnTable ()->AddFtn (ftn);
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe1, policy.CreatePolicy ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  return GetFtnTable ()->AddFtn (ftn);
}

template<class T>  
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
  const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe1, policy.CreatePolicy ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  return GetFtnTable ()->AddFtn (ftn);
}

template<class T>                    
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
  const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe1, policy.CreatePolicy ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  return GetFtnTable ()->AddFtn (ftn);
}

template<class T>                    
uint32_t
MplsFtnHelper::AddFtn (const T &fec, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
  const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (mpls::Fec::Build (fec), nhlfe1, policy.CreatePolicy ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  return GetFtnTable ()->AddFtn (ftn);
}


} // namespace ns3

#endif /* MPLS_FTN_HELPER_H */
