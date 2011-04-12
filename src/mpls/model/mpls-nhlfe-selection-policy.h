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

#ifndef MPLS_NHLFE_SELECTION_POLICY_H
#define MPLS_NHLFE_SELECTION_POLICY_H

#include "ns3/object.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief Abstract NHLFE selection policy
 */
class NhlfeSelectionPolicy : public Object
{
public:
  static TypeId GetTypeId (void);
  
  class Info 
  {
  public:
    Info ();
    virtual ~Info ();
  };
  
  NhlfeSelectionPolicy ();
  virtual ~NhlfeSelectionPolicy ();
  
  /**
   * @return NHLFE for specified index (called by the Iterator)
   */
  virtual Nhlfe& GetNhlfe (uint32_t index);
  /**
   * @return New policy information object (called by the ForwardingInformation)
   */
  virtual Info* CreatePolicyInfo ();
  
  Iterator Begin ();
  Iterator End ();
  
protected:
  virtual void DoDispose (void);
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_SELECTION_POLICY_H */
