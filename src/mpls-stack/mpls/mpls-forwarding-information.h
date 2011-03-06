/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
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
 */

#ifndef MPLS_FORWARDING_INFORMATION_H
#define MPLS_FORWARDING_INFORMATION_H

#include <vector>
#include <ostream>

#include "ns3/simple-ref-count.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

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
  Nhlfe& GetNhlfe (uint32_t index);
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
   * @brief Print NHLFE
   * @param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  ForwardingInformation ();

protected:
  typedef std::vector<Nhlfe> NhlfeVector;
  NhlfeVector m_nhlfe;
};

/**
 * \brief output operation for ForwardingInformation
 */
std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformation>& info);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_H */
