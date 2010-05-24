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

#include <iosteam>
#include "mpls-ilm.h"

namespace ns3 {
namespace mpls {

MplsIlm::MplsIlm ()
  : m_interface (0)
{
}

MplsIlm::MplsIlm (const MplsLabel &label, const Ptr<MplsInterface> &interface);
  : m_interface (interface),
    m_label (label)
{
}

MplsIlm::~MplsIlm ()
{
  m_interface = 0;
}

void
MplsIlm::SetLabel (const MplsLabel &label)
{
  m_label = label;
}

const MplsLabel&
MplsIlm::GetLabel (void) const
{
  return m_label;
}

void
MplsIlm::SetInterface (const Ptr<MplsInterface> &interface)
{
  m_interface = interface;
}

Ptr<MplsInterface>
MplsIlm::GetInterface (void) const
{
  return m_interface;
}

void
MplsIlm::Print (std::ostream &os) const
{
}

} // namespace mpls
} // namespace ns3
