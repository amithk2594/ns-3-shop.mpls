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

using namespace mpls;

MplsFtnHelper::~MplsFtnHelper()
{
}

void
MplsFtnHelper::RemoveFtn (const Ptr<FecToNhlfe> &ftn)
{
  GetNode ()->GetFtnTable ()->remove (ftn);
}

void 
MplsFtnHelper::ClearFtnTable ()
{
  GetNode ()->GetFtnTable ()->clear ();
}

} // namespace mpls
