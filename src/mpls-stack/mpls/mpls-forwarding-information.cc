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

#include "mpls-forwarding-information.h"

namespace ns3 {
namespace mpls {

ForwardingInformation::ForwardingInformation ()
{
}

ForwardingInformation::~ForwardingInformation ()
{
  m_nhlfe.clear ();
}

uint32_t
ForwardingInformation::AddNhlfe (const Nhlfe& nhlfe)
{
  uint32_t index = m_nhlfe.size ();
  m_nhlfe.push_back (nhlfe);
	return index;
}

void
ForwardingInformation::RemoveNhlfe (uint32_t index)
{
	NS_ASSERT_MSG (index < m_nhlfe.size (), 
									"ForwardingInformation::RemoveNhlfe (): invalid NHLFE index");
  m_nhlfe.erase (m_nhlfe.begin () + index);
}

uint32_t
ForwardingInformation::GetNNhlfe (void) const
{
  return m_nhlfe.size ();
}

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformation>& info)
{
  info->Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
