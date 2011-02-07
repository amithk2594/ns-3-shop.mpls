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

#include <list>
#include <ostream>

#include "ns3/simple-ref-count.h"
#include "mpls-next-hop-label-forwarding-entry.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief A MPLS forwarding information base class.
 */
class ForwardingInformation : public SimpleRefCount<ForwardingInformation>
{
public:
  /**
   * \brief Destructor
   */
  virtual ~ForwardingInformation ();
  /**
   * \brief Adds new NHLFE
   * \param nhlfe Next Hop Label Forwarding Entry
   */
  void AddNhlfe (const Ptr<NextHopLabelForwardingEntry> &nhlfe);
  /**
   * \brief Get NHLFE by index
   * \param index
   * \returns Next Hop Label Forwarding Entry
   */
  const Ptr<NextHopLabelForwardingEntry>& GetNhlfe (uint32_t index);
  /**
   * \brief Remove the specific NHLFE from the list
   * \param nhlfe NHLFE
   * \returns true if NHLFE removed
   */
  bool RemoveNhlfe (const Ptr<NextHopLabelForwardingEntry> &nhlfe);
  /**
   * \brief Get NHLFE's count
   * \returns count of NHLFE
   */
  uint32_t GetNNhlfe (void) const;
  /**
   * \brief Print NHLFE
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  ForwardingInformation ();

protected:
  typedef std::list<Ptr<NextHopLabelForwardingEntry> > NhlfeList;
  NhlfeList m_nhlfeList;
};

/**
 * \brief output operation for ForwardingInformation
 */
std::ostream& operator<< (std::ostream& os, const ForwardingInformation &info);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_H */
