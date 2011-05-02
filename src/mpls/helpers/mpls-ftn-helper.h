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
#include "ns3/mpls.h"
#include "ns3/mpls-label.h"
#include "ns3/mpls-ftn-table.h"
#include "ns3/mpls-fec-to-nhlfe.h"
#include "ns3/mpls-fec.h"
#include "ns3/mpls-nhlfe.h"

#include "mpls-node-helper-base.h"

namespace ns3 {

using namespace mpls;

/**
 * \brief Mpls ftn table helper
 */
class MplsFtnHelper : public MplsNodeHelperBase
{
public:
  virtual ~MplsFtnHelper();
  /**
   * @brief Remove FTN
   */
  void RemoveFtn (const Ptr<FecToNhlfe> &ftn);
  /**
   * @brief Clear FTN table
   */
  void ClearFtnTable ();
  /**
   * @brief Add a new FTN
   * @param fec FEC
   * @param nhlfe NHLFE
   * @return FTN 
   */
  template<class T>
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10, const Nhlfe &nhlfe11);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10, const Nhlfe &nhlfe11, const Nhlfe &nhlfe12);

  /**
   * @brief Add a new FTN
   * @param fec FEC
   * @param nhlfe NHLFE
   * @param Nhlfe selection policy
   * @return FTN 
   */
  template<class T>
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>  
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
                    const NhlfeSelectionPolicyHelper& policy);
  template<class T>                    
  Ptr<FecToNhlfe> AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
                    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
                    const Nhlfe &nhlfe7, const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, 
                    const Nhlfe &nhlfe10, const Nhlfe &nhlfe11, const Nhlfe &nhlfe12,
                    const NhlfeSelectionPolicyHelper& policy);
};

template<class T>
Ptr<FecToNhlfe> 
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe)
{
  return AddFtn (fec, nhlfe, GetSelectionPolicy ());
}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2)
{
  return AddFtn (fec, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
    const Nhlfe &nhlfe8)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
    const Nhlfe &nhlfe8, const Nhlfe &nhlfe9)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
    const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10,
      GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
    const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10,
      nhlfe11, GetSelectionPolicy ());
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
    const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
    const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
    const Nhlfe &nhlfe12)
{
  return AddFtn (fec, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10,
      nhlfe11, nhlfe12, GetSelectionPolicy ());
}

template<class T>
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe, policy.Create ());
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;

}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>  
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const Nhlfe &nhlfe8,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  ftn->AddNhlfe (nhlfe8);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const Nhlfe &nhlfe8, const Nhlfe &nhlfe9,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  ftn->AddNhlfe (nhlfe8);
  ftn->AddNhlfe (nhlfe9);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  ftn->AddNhlfe (nhlfe8);
  ftn->AddNhlfe (nhlfe9);
  ftn->AddNhlfe (nhlfe10);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}


template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  ftn->AddNhlfe (nhlfe8);
  ftn->AddNhlfe (nhlfe9);
  ftn->AddNhlfe (nhlfe10);
  ftn->AddNhlfe (nhlfe11);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

template<class T>                    
Ptr<FecToNhlfe>
MplsFtnHelper::AddFtn (const T &fec, const Nhlfe &nhlfe1, const Nhlfe &nhlfe2, const Nhlfe &nhlfe3, 
  const Nhlfe &nhlfe4, const Nhlfe &nhlfe5, const Nhlfe &nhlfe6, const Nhlfe &nhlfe7,
  const Nhlfe &nhlfe8, const Nhlfe &nhlfe9, const Nhlfe &nhlfe10, const Nhlfe &nhlfe11,
  const Nhlfe &nhlfe12,
  const NhlfeSelectionPolicyHelper& policy)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (Fec::Build (fec), nhlfe1, policy.Create ());
  ftn->AddNhlfe (nhlfe2);
  ftn->AddNhlfe (nhlfe3);  
  ftn->AddNhlfe (nhlfe4);  
  ftn->AddNhlfe (nhlfe5);  
  ftn->AddNhlfe (nhlfe6);
  ftn->AddNhlfe (nhlfe7);
  ftn->AddNhlfe (nhlfe8);
  ftn->AddNhlfe (nhlfe9);
  ftn->AddNhlfe (nhlfe10);
  ftn->AddNhlfe (nhlfe11);
  ftn->AddNhlfe (nhlfe12);
  GetNode ()->GetFtnTable ()->push_back (ftn);
  return ftn;
}

} // namespace ns3

#endif /* MPLS_FTN_HELPER_H */
