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

#ifndef MPLS_NHLFE_H
#define MPLS_NHLFE_H

#include <ostream>
#include <vector>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-generic.h"
#include "mpls-operations.h"
#include "mpls-interface.h"

namespace ns3 {
namespace mpls {

class Interface;

/**
 * \ingroup mpls
 * \brief A representation of the "Next Hop Label Forwarding Entry" (NHLFE)
 *
 * The "Next Hop Label Forwarding Entry" (NHLFE) is used to forward a labeled packet.
 */
class NextHopLabelForwardingEntry : public SimpleRefCount<NextHopLabelForwardingEntry>
{
public:
  /**
   * \brief construct empty NHLFE
   */
  NextHopLabelForwardingEntry ();
  /**
   * \brief construct NHLFE with specified operation vector and interface
   * \param interface MplsInterface
   * \param op MplsOp
   */
  NextHopLabelForwardingEntry (const Ptr<MplsInterface> &interface, const MplsOp& op);
  /**
   * \brief Destructor
   */
  virtual ~NextHopLabelForwardingEntry ();
  /**
   * \brief Set operation vector
   * \param op MplsOp
   */
  void SetMplsOp (const Ptr<MplsOp>& op);
  /**
   * \brief Get operation vector
   * \returns MplsOp
   */
  Ptr<MplsOp>& GetOp ();
  /**
   * \brief Get operation vector
   * \returns MplsOp
   */
  const Ptr<MplsOp>& GetOp () const;
  /**
   * \brief Set outgoing interface
   * \param interface MplsInterface
   */
  void SetInterface (const Ptr<MplsInterface> &interface);
  /**
   * \brief Get outgoing interface
   * \returns MplsInterface
   */
  const Ptr<MplsInterface>& GetInterface (void) const;
  /**
   * \brief print NHLFE
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  Ptr<MplsInterface> m_interface;
  Ptr<MplsOp>        m_op;
};

/**
 * \brief output operation for NextHopLabelForwardingEntry
 */
std::ostream& operator<< (std::ostream& os, const Ptr<NextHopLabelForwardingEntry> &nhlfe);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_H */
