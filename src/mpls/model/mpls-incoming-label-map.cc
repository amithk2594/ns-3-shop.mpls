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
 
#include <iostream>
#include "mpls-incoming-label-map.h"

namespace ns3 {
namespace mpls {

IncomingLabelMap::IncomingLabelMap (Label label, const Nhlfe &nhlfe)
  : m_interface (0),
    m_label (label)
{
  AddNhlfe (nhlfe);
}

IncomingLabelMap::IncomingLabelMap (uint32_t interface, Label label, const Nhlfe &nhlfe)
  : m_interface (interface),
    m_label (label)
{
  AddNhlfe (nhlfe);
}

IncomingLabelMap::~IncomingLabelMap ()
{
}

Label
IncomingLabelMap::GetLabel (void) const
{
  return m_label;
}

uint32_t
IncomingLabelMap::GetInterface (void) const
{
  return m_interface;
}

void
IncomingLabelMap::SetLabel (Label label)
{
  m_label = label;
}

void
IncomingLabelMap::SetInterface (uint32_t interface)
{
  m_interface = interface;
}

void
IncomingLabelMap::Print (std::ostream &os) const
{
}

} // namespace mpls
} // namespace ns3
