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

#include "mpls-ilm-table.h"

namespace ns3 {
namespace mpls {


uint32_t IlmTable::ilmId = 0;
  
IlmTable::IlmTable ()
{
}

IlmTable::~IlmTable ()
{
}

uint32_t
IlmTable::AddIlm (const Ptr<IncomingLabelMap>& ilm)
{
  m_ilmTable[++ilmId] = ilm;
  
  return ilmId;
}

Ptr<IncomingLabelMap>
IlmTable::GetIlm (const uint32_t index)
{
  return m_ilmTable[index];
}

void
IlmTable::RemoveIlm (const uint32_t index)
{
  m_ilmTable.erase (index);
}

void
IlmTable::Clear (void)
{
  m_ilmTable.clear ();
}
  
void
IlmTable::Print (std::ostream& os) const
{
}

IlmTable::Iterator
IlmTable::Begin (void) const
{
  return m_ilmTable.begin ();
}

IlmTable::Iterator
IlmTable::End (void) const
{
  return m_ilmTable.end ();
}

std::ostream& operator<< (std::ostream& os, const Ptr<IlmTable> &ilm)
{
  ilm->Print (os);
  
  return os;
}

} // namespace mpls
} // namespace ns3
