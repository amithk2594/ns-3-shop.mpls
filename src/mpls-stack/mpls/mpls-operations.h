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

#ifndef MPLS_OPERATIONS_H
#define MPLS_OPERATIONS_H

#include <ostream>
#include "mpls-generic.h"
#include "mpls-label-stack.h"

namespace ns3 {

namespace mpls {
namespace op {
/**
 * \ingroup mpls
 * \brief A MPLS 'label push' operation.
 */
class Push : public MplsOperation
{
public:
  /**
   * \brief Create a label push operation.
   * \param label MplsLabel
   */
  Push (const MplsLabel &label);
  /**
   * \brief Destructor
   */
  virtual ~Push ();

  // Functions defined in base class MplsOperation
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;

private:
  MplsLabel m_label;
};

/**
 * \ingroup mpls
 * \brief A MPLS 'label pop' operation.
 */
class Pop : public MplsOperation
{
public:
  /**
   * \brief Create a label pop operation.
   */
  Pop ();
  /**
   * \brief Destructor
   */
  virtual ~Pop ();

  // Functions defined in base class MplsOperation
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;
};

/**
 * \ingroup mpls
 * \brief A MPLS 'label swap' operation.
 */
class Swap : public MplsOperation
{
public:
 /**
   * \brief Create a label swap operation.
   * \param label MplsLabel
   */
  Swap (const MplsLabel &label);
  /**
   * \brief Destructor
   */
  virtual ~Swap ();

  // Functions defined in base class MplsOperation
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;

private:
  MplsLabel m_label;
};

} // namespace op

/**
 * \ingroup mpls
 * \brief Mpls label forwarding operation vector
 *
 * This class is used for creating operation sequence
 */
class MplsOp
{
public:
  /**
   * \brief Create empty operation vector
   */
  MplsOp ();
  /**
   * \brief Destructor
   */
  virtual ~MplsOp ();
  /**
   * \brief Add custom label operation.
   * \param oper MplsOperation
   * \returns this MplsOp object
   */
  MplsOp& AddOp (const op::MplsOperation &oper);
  /**
   * \brief Push label operation shortcut
   * \param label MplsLabel
   * \returns this MplsOp object
   */
  MplsOp& Push (const MplsLabel &label);
  /**
   * \brief Pop label operation shortcut
   * \returns this MplsOp object
   */
  MplsOp& Pop ();
  /**
   * \brief Swap label operation shortcut
   * \param label MplsLabel
   * \returns this MplsOp object
   */
  MplsOp& Swap (const MplsLabel &label);

  typedef std::vector<MplsOperation>::const_iterator Iterator;
  /**
   * \brief Get an iterator which refers to the first operation
   */
  Iterator Begin (void) const;
  /**
   * \brief Get an iterator which indicates past-the-last operation
   */
  Iterator End (void) const;
//  /**
//   * \brief Perform operations under the label stack
//   * \param stack mpls label stack to perform operations
//   * \returns false if executing fails
//   */
//  bool Execute (MplsLabelStack &stack) const;
  /**
   * \brief Print operation vector
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  typedef std::vector<MplsOperation> OperationVector;
  OperationVector m_operations;
};

/**
 * \brief output operation for MplsOp
 */
std::ostream& operator<< (std::ostream& os, const MplsOp &op);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_OPERATIONS_H */
