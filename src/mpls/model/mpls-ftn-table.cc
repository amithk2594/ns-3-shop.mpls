/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#include "mpls-ftn-table.h"

namespace ns3 {
namespace mpls {


uint32_t FtnTable::ftnId = 0;
  
FtnTable::FtnTable ()
{
}

FtnTable::~FtnTable ()
{
}

template <class T>
uint32_t
FtnTable::AddFtn (int32_t deviceIndex, const T& fec, const Nhlfe& nhlfe)
{
  Ptr<FecToNhlfe> ftn = Create<FecToNhlfe> (deviceIndex, fec, nhlfe);
  
  return AddFtn (ftn);
}


template <class T>
uint32_t
FtnTable::AddFtn (const T& fec, const Nhlfe& nhlfe)
{
  return AddFtn (-1, fec, nhlfe);
}

uint32_t
FtnTable::AddFtn (const Ptr<FecToNhlfe> &ftn)
{
  m_ftnTable[++ftnId] = ftn;
  
  return ftnId;
}

Ptr<FecToNhlfe>
FtnTable::GetFtn (const uint32_t index)
{
  return m_ftnTable[index];
}

void
FtnTable::RemoveFtn (const uint32_t index)
{
  m_ftnTable.erase (index);
}

void
FtnTable::Print (std::ostream &os) const
{
}

FtnTable::Iterator
FtnTable::Begin (void ) const
{
  return m_ftnTable.begin ();
}

FtnTable::Iterator
FtnTable::End (void ) const
{
  return m_ftnTable.end ();
}

std::ostream& operator<< (std::ostream& os, const Ptr<FtnTable> &ftn)
{
  ftn->Print (os);
  
  return os;
}

} // namespace mpls
} // namespace ns3
