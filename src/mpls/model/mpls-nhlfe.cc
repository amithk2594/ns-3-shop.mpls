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

Nhlfe::Nhlfe ()
  : m_interface (0)
{
}

Nhlfe::Nhlfe (const Ptr<MplsInterface>& interface, const OperationVector& op)
  : m_interface (interface),
    m_operations (op)
{
}

Nhlfe::~Nhlfe ()
{
  m_interface = 0;
}

void
Nhlfe::SetOperations (const OperationVector& op)
{
  m_operations = op;
}

const OperationVector&
Nhlfe::GetOperations (void) const
{
  return m_operations;
}

void
Nhlfe::SetInterface (const Ptr<MplsInterface> &interface)
{
  m_interface = interface;
}

const Ptr<MplsInterface>&
Nhlfe::GetInterface (void) const
{
  return m_interface;
}

void
Nhlfe::Print (std::ostream &os) const
{
//  os << m_interface
//     << "operations: " << m_operations << std::endl
//    ;
}

std::ostream& operator<< (std::ostream& os, const Nhlfe& nhlfe)
{
  nhlfe.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
