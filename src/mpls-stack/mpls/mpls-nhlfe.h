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

#include "mpls-label.h"
#include "mpls-operations.h"
#include "mpls-interface.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief Mpls operation vector to perform on the packet's label stack.
 */
class MplsOp
{
public:
  /**
   * \brief Create empty operation vector
   */
  MplsOp ();
  virtual ~MplsOp ();
  /**
   * \brief add custom stack operation
   * \param oper operation
   * \returns this MplsOp object
   */
  MplsOp& AddOp (const op::MplsOp &oper);
  /**
   * \brief Push operation shortcut
   * \param label
   * \returns this MplsOp object
   */
  MplsOp& Push (const op::MplsLabel &label);
  /**
   * \brief Pop operation shortcut
   * \returns this MplsOp object
   */
  MplsOp& Pop ();
  /**
   * \brief Swap operation shortcut
   * \param label label
   * \returns this MplsOp object
   */
  MplsOp& Swap (const op::MplsLabel &label);
  /**
   * \brief Execute operations on the label stack
   * \param stack mpls label stack to perform operations
   * \returns false if executing fails
   */
  bool Execute (MplsLabelStack &stack) const;
  /**
   * \brief print operations
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  typedef std::vector<op::MplsOpBase> OperationVector;
  OperationVector m_operations;
};

/**
 * \ingroup mpls
 * \brief
 * The "Next Hop Label Forwarding Entry" (NHLFE) is used when forwarding a labeled packet.
 */
class MplsNhlfe : public SimpleRefCount<MplsNhlfe>
{
public:
  /**
   * \brief construct empty NHLFE
   */
  MplsNhlfe ();
  /**
   * \brief construct NHLFE with specified operations, interface and lspid
   * \param lspid
   * \param interface
   * \param oper
   */
  MplsNhlfe (uint16_t lspid, const Ptr<MplsInterface> &interface, const MplsOp& oper);

  virtual ~MplsNhlfe ();
  /**
   * \brief assign operations
   * \param opers operations
   */
  void SetOp (const MplsOp& op);
  /**
   * \returns operation vector
   */
  MplsOp& GetOp ();
  /**
   * \returns operation vector
   */
  const MplsOp& GetOp () const;
  /**
   * \brief set outgoing interface
   * \param ifIndex outgoing interface index
   */
  void SetInterface (const Ptr<MplsInterface> &interface);
  /**
   * \returns outgoing interface index
   */
  Ptr::MplsInterface GetInterface (void) const;
  /**
   * \brief set local lsp id
   * \param lspid lsp id
   */
  void SetLspId (uint16_t lspid);
  /**
   * \returns local lsp id
   */
  uint16_t GetLspId (void) const;
  /**
   * \brief set next-hop lsr address
   * \param nextHop next-hop lsr address
   * For future use
   */
  //void SetNextHop (const Address &nextHop);
  /**
   * \returns next-hop lsr address
   * For future use
   */
  //Address GetNextHop (void) const;
  /**
   * \brief print NHLFE
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  Ptr<MplsInterface> m_interface;
  uint16_t           m_lspid;
  MplsOp             m_operations;
//  Address m_nextHop;
};

std::ostream& operator<< (std::ostream& os, const MplsNhlfe &nhlfe);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_H */
