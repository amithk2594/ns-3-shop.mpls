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
#include "ns3/assert.h"

namespace ns3 {
namespace mpls {

OperationBuilder::OperationBuilder ()
{
}

OperationBuilder::~OperationBuilder ()
{
}

OperationBuilder&
OperationBuilder::Push (Label label)
{
  m_operations->push_back (OP_PUSH);
  m_operations->push_back (label);
  return *this;
}

OperationBuilder&
OperationBuilder::Pop ()
{
  m_operations->push_back (OP_POP);
  return *this;
}

OperationBuilder&
OperationBuilder::Swap (Label label)
{
  m_operations->push_back (OP_SWAP);
  m_operations->push_back (label);
  return *this;
}

const OperationVector&
OperationBuilder::GetOperations (void) const
{
  return m_operations;
}

OperationIterator::OperationIterator (const OperationVector& op)
{
  m_start = op.begin ();
  m_end = op.end ();
}

OperationIterator::~OperationIterator ()
{
}

bool
OperationIterator::HasNext () const
{
  return m_start != m_end;
}

uint32_t
OperationIterator::Get ()
{
  NS_ASSERT_MSG (HasNext (), "mpls::OperationIterator(): malformed operation vector");
  return *m_start++;
}

} // namespace mpls
} // namespace ns3
