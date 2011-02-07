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

const int32_t MplsOp::PUSH = 0;
const int32_t MplsOp::POP = 1;
const int32_t MplsOp::SWAP = 2;
  
MplsOp::MplsOp ()
{
}

MplsOp::~MplsOp ()
{
  m_operations.clear ();
}

MplsOp::Vector&
MplsOp::GetOpVector ()
{
  return m_operations;
}

namespace op {

void
Push (Ptr<MplsOp> &v, Label label)
{
  MplsOp::Vector& vec = v->GetOpVector ();
  vec.push_back (MplsOp::PUSH);
  vec.push_back (label);
}

void
Pop (Ptr<MplsOp> &v)
{
  v->GetOpVector ().push_back (MplsOp::POP);
}

void
Swap (Ptr<MplsOp> &v, Label label)
{
  MplsOp::Vector& vec = v->GetOpVector ();
  vec.push_back (MplsOp::SWAP);
  vec.push_back (label);
}

} // namespace op 


} // namespace mpls
} // namespace ns3
