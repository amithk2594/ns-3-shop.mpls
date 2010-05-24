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
  : m_interface (0),
    m_lspid (-1)
{
}

MplsNhlfe::MplsNhlfe (uint16_t lspid, const Ptr<MplsInterface> &interface, const MplsOp& oper);
  : m_interface (interface),
    m_lspid (lspid),
    m_operations (oper)
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

Ptr<MplsInterface>
MplsNhlfe::GetInterface (void) const
{
  return m_interface;
}

void
MplsNhlfe::SetLspId (uint16_t lspid)
{
  m_lspid = lspid;
}

uint16_t
MplsNhlfe::GetLspId (void) const
{
  return m_lspid;
}

void
MplsNhlfe::Print (std::ostream &os) const
{
  os << "lspid: " << (uint32_t)m_lspid << std::endl
     << "operations: " << m_operations << std::endl
     << *m_interface
    ;
}

std::ostream& operator<< (std::ostream& os, const MplsNhlfe &nhlfe)
{
  nhlfe.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
