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

#include "ns3/assert.h"
#include "ns3/log.h"

#include "mpls-ftn-helper.h"

namespace ns3 {

MplsFtnHelper::~MplsFtnHelper()
{
}

Ptr<mpls::FtnTable>
MplsFtnHelper::GetFtnTable (void) const
{
  return GetMpls ()->GetFtnTable ();
}

void 
MplsFtnHelper::SetFtnTable (const Ptr<mpls::FtnTable> &table)
{
  NS_ASSERT (table != 0);
  GetMpls ()->SetFtnTable (table);  
}

Ptr<mpls::FecToNhlfe> 
MplsFtnHelper::GetFtn (const uint32_t index) const
{
  return GetFtnTable ()->GetFtn (index);
}

void
MplsFtnHelper::RemoveFtn (const uint32_t index)
{
  GetFtnTable ()->RemoveFtn (index);
}

void 
MplsFtnHelper::ClearFtnTable ()
{
  GetFtnTable ()->Clear ();
}

} // namespace mpls
