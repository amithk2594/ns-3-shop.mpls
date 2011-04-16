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

#include <vector>
#include "ns3/object.h"
#include "ns3/packet.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

class Nhlfe;

/**
 * \ingroup mpls
 * \brief Abstract NHLFE selection policy
 */
class NhlfeSelectionPolicy : public Object
{
public:
  static TypeId GetTypeId (void);
  
  NhlfeSelectionPolicy ();
  virtual ~NhlfeSelectionPolicy ();
  
  /**
   * @brief Returns NHLFE for specified index (called by the Iterator)
   */
  virtual const Nhlfe& Get (const std::vector<Nhlfe> &nhlfe, uint32_t index);
  /**
   * @brief Returns true if nhlfe can be selected
   * @param nhlfe Nhlfe vector
   * @param index Nhlfe index
   * @param iface Outgoing interface
   * @param packet Packet
   */
  virtual bool Select (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
      const Ptr<const Interface> &interface, const Ptr<const Packet> &packet);   
  /**
   * @brief Print policy 
   */
  virtual void Print (std::ostream &os) const;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_SELECTION_POLICY_H */
