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

#ifndef MPLS_FORWARDING_INFORMATION_H
#define MPLS_FORWARDING_INFORMATION_H

#include <vector>
#include <ostream>

#include "ns3/simple-ref-count.h"
#include "mpls-nhlfe.h"
#include "mpls-nhlfe-selection-policy.h"

namespace ns3 {
namespace mpls {

class Nhlfe;
class NhlfeSelectionPolicy;

/**
 * \ingroup mpls
 * \brief A MPLS forwarding information
 */
class ForwardingInformation : public SimpleRefCount<ForwardingInformation>
{
public:
  /**
   * @brief Destructor
   */
  virtual ~ForwardingInformation ();
  /**
   * @brief Add new NHLFE
   * @param nhlfe Next Hop Label Forwarding Entry
	 * @return NHLFE index
   */
  uint32_t AddNhlfe (const Nhlfe& nhlfe);
  /**
   * @brief Get NHLFE by index
   * @param index NHLFE index
   */
  const Nhlfe& GetNhlfe (uint32_t index);
  /**
   * @brief Remove the specific NHLFE
   * @param nhlfe NHLFE
   */
  void RemoveNhlfe (uint32_t index);
  /**
   * @brief Get NHLFE count
   * @return NHLFE count
   */
  uint32_t GetNNhlfe (void) const;
  /**
   * @brief Get object index
   */
  uint32_t GetIndex (void) const;
  /**
   * @brief Set object index
   */
  void SetIndex (uint32_t index);
  /**
   * @brief Set nhlfe selection policy
   */
  void SetPolicy (const Ptr<NhlfeSelectionPolicy> &policy);
  /**
   * @brief Set nhlfe selection policy
   */
  Ptr<NhlfeSelectionPolicy> GetPolicy (void) const;
  /**
   * @brief Print NHLFE
   */
  virtual void Print (std::ostream &os) const = 0;

  typedef std::vector<Nhlfe> NhlfeVector;
  
  class Iterator : public std::iterator<std::input_iterator_tag, Nhlfe> 
  {
  public:
    Iterator(NhlfeSelectionPolicy *policy, NhlfeVector *nhlfe, uint32_t index=0);
    ~Iterator();

    Iterator& operator=(const Iterator& iter);
    bool operator==(const Iterator& iter);
    bool operator!=(const Iterator& iter);
    Iterator& operator++ ();
    Nhlfe& operator*();

  private:
    NhlfeSelectionPolicy* m_policy;
    NhlfeVector *vector;
    uint32_t m_index;
  };

  Iterator Begin (void) const;
  Iterator End (void) const;

protected:
  ForwardingInformation ();
  NhlfeVector m_nhlfe;
  uint32_t m_index;
  
  Ptr<NhlfeSelectionPolicy> m_policy;
};

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformation>& info);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_H */
