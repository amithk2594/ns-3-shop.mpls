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
#include "mpls-label.h"
#include "mpls-label-stack.h"

namespace ns3 {
namespace mpls {
namespace op {

/**
 * \ingroup mpls
 * \brief Mpls operation to perform on the packet's label stack.
 */
class MplsOpBase
{
public:
  virtual ~MplsOpBase ();
  /**
   * \brief Execute operation on the label stack
   * \param stack mpls label stack to perform operation
   * \returns false if operation executing failed
   */
  virtual bool Execute (MplsLabelStack &stack) const = 0;
  /**
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  MplsOpBase ();
};

std::ostream& operator<< (std::ostream& os, const MplsOpBase &op);

/**
 * \ingroup mpls
 * \brief Push operation.
 */
class Push : public MplsOpBase
{
public:
  /**
   * \brief Create a push operation.
   * \param label operation label
   */
  Push (const MplsLabel &label);
  virtual ~Push ();
  // Functions defined in base class MplsOpBase
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;

private:
  MplsLabel m_label;
}

/**
 * \ingroup mpls
 * \brief Pop operation.
 */
class Pop : public MplsOpBase
{
public:
  /**
   * \brief Create a pop operation.
   */
  Pop ();
  virtual ~Pop ();
  // Functions defined in base class MplsOpBase
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;
}

/**
 * \ingroup mpls
 * \brief Pop operation.
 */
class Swap : public MplsOpBase
{
public:
  /**
   * \brief Create a swap operation.
   * \param label operation label.
   */
  Swap (const MplsLabel &label);
  virtual ~Swap ();
  // Functions defined in base class MplsOpBase
  virtual bool Execute (MplsLabelStack &stack) const;
  virtual void Print (std::ostream &os) const;

private:
  MplsLabel m_label;
}

} // namespace op
} // namespace mpls
} // namespace ns3

#endif /* MPLS_OPERATIONS_H */
