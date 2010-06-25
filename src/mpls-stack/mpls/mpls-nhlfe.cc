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
#include <iostream>
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

MplsNhlfe::MplsNhlfe ()
  : m_interface (0)
{
}

MplsNhlfe::MplsNhlfe (const Ptr<MplsInterface> &interface, const MplsOp& op);
  : m_interface (interface),
    m_operations (op)
{
}

MplsNhlfe::~MplsNhlfe ()
{
  m_interface = 0;
}

void
MplsNhlfe::SetOp (const MplsOp& op)
{
  m_operations = op;
}

MplsOp&
MplsNhlfe::GetOp ()
{
  return m_operations;
}

const MplsOp&
MplsNhlfe::GetOp () const
{
  return m_operations;
}

void
MplsNhlfe::SetInterface (const Ptr<MplsInterface> &interface)
{
  m_interface = interface;
}

const Ptr<MplsInterface>&
MplsNhlfe::GetInterface (void) const
{
  return m_interface;
}

void
MplsNhlfe::Print (std::ostream &os) const
{
  os << m_interface
     << "operations: " << m_operations << std::endl
    ;
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsNhlfe> &nhlfe)
{
  nhlfe->Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
