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

/**
 * \ingroup mpls
 * \brief A representation of the "Next Hop Label Forwarding Entry" (NHLFE)
 *
 * The "Next Hop Label Forwarding Entry" (NHLFE) is used to forward a labeled packet.
 */
class MplsNhlfe : public SimpleRefCount<MplsNhlfe>
{
public:
  /**
   * \brief construct empty NHLFE
   */
  MplsNhlfe ();
  /**
   * \brief construct NHLFE with specified operation vector and interface
   * \param interface MplsInterface
   * \param op MplsOp
   */
  MplsNhlfe (const Ptr<MplsInterface> &interface, const MplsOp& op);
  /**
   * \brief Destructor
   */
  virtual ~MplsNhlfe ();
  /**
   * \brief Set operation vector
   * \param op MplsOp
   */
  void SetOp (const MplsOp& op);
  /**
   * \brief Get operation vector
   * \returns MplsOp
   */
  MplsOp& GetOp ();
  /**
   * \brief Get operation vector
   * \returns MplsOp
   */
  const MplsOp& GetOp () const;
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
  MplsOp             m_operations;
};

/**
 * \brief output operation for MplsNhlfe
 */
std::ostream& operator<< (std::ostream& os, const MplsNhlfe &nhlfe);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_H */
