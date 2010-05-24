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

#include "mpls-operations.h"

namespace ns3 {
namespace mpls {
namespace op {

MplsOpBase::MplsOpBase ()
{
}

MplsOpBase::~MplsOpBase ()
{
}

/// Push
Push::Push (const MplsLabel &label)
  : m_label (label)
{
  NS_ASSERT_MSG (!m_label.IsInvalid (), "mpls::op::Push (): invalid label");
}

Push::~Push ()
{
}

bool
Push::Execute (MplsLabelStack &stack) const
{
  //XXX: copy exp value from the top???
  stack.Push (MplsStackEntry (m_label));
  return true;
}

void
Push::Print (std::ostream &os) const
{
  os << "push " << m_label;
}

/// Pop
Pop::Pop ()
{
}

Pop::~Pop ()
{
}

void
Pop::Execute (MplsLabelStack &stack) const
{
  if (Stack.IsEmpty ())
    {
      return false;
    }

  stack.Pop ();
  return true;
}

void
Pop::Print (std::ostream &os) const
{
  os << "pop";
}

/// Swap
Swap::Swap (const MplsLabel &label)
  : m_label (label)
{
  NS_ASSERT_MSG (!m_label.IsInvalid (), "mpls::op::Swap (): invalid label");
}

Swap::~Swap ()
{
}

bool
Swap::Execute (MplsLabelStack &stack) const
{
  if (Stack.IsEmpty ())
    {
      return false;
    }

  //XXX: copy exp value???
  stack.Pop ();
  stack.Push (MplsStackEntry (m_label));
  return true;
}

void
Swap::Print (std::ostream &os) const
{
  os << "swap " << m_label;
}

std::ostream& operator<< (std::ostream& os, const MplsOpBase &op)
{
  op.Print (os);
  return os;
}

} // namespace op
} // namespace mpls
} // namespace ns3
