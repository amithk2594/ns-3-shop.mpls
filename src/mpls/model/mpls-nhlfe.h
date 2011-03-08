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
class Nhlfe
{
public:
  /**
   * @brief construct NHLFE with specified operation and outgoing interface
   * @param outInterface Outgoing interface index
   * @param op Operation to perform
   */
  Nhlfe (const Operation& op, uint32_t outInterface);
  /**
   * @brief construct NHLFE with specified operation and next-hop
   * @param nextHop next-hop
   * @param op Operation to perform
   */
  Nhlfe (const Operation& op, const Address& nextHop);
  /**
   * @brief Destructor
   */
  virtual ~Nhlfe ();
  /**
   * @brief Get outgoing interface
   */
  uint32_t GetInterface (void) const;
  /**
   * @brief Get next-hop
   */
  const Address* GetNextHop (void) const;
  /**
   * @brief print NHLFE
   * @param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  uint32_t m_interface;
  Address m_nextHop;
  uint32_t m_opcode;
  uint32_t m_count;
  uint32_t m_labels[6];
  
  friend class Swap;
  friend class Pop;
};

/**
 * @brief output operation for NHLFE
 */
std::ostream& operator<< (std::ostream& os, const Nhlfe& nhlfe);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_H */
