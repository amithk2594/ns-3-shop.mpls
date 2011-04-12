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

#include "mpls-forwarding-information.h"

#include "ns3/log.h"
#include "ns3/assert.h"

namespace ns3 {
namespace mpls {

ForwardingInformation::ForwardingInformation ()
  : m_index (0),
    m_policy (0),
    m_info (0)
{
}

ForwardingInformation::~ForwardingInformation ()
{
  delete m_info;
  m_policy = 0;
  m_nhlfe.clear ();
}

void 
ForwardingInformation::SetPolicy (const Ptr<NhlfeSelectionPolicy> &policy)
{
  if (m_policy != policy)
    {
      m_policy = policy;
      delete m_info;
      m_info = policy->CreateInfo ();
    } 
}

Ptr<NhlfeSelectionPolicy>
ForwardingInformation::GetPolicy (void) const
{
  return m_policy;
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
  NS_ASSERT_MSG (index < m_nhlfe.size (), "Invalid NHLFE index");
  m_nhlfe.erase (m_nhlfe.begin () + index);
}

uint32_t
ForwardingInformation::GetNNhlfe (void) const
{
  return m_nhlfe.size ();
}

uint32_t
ForwardingInformation::GetIndex (void) const
{
  return m_index;
}

void
ForwardingInformation::SetIndex (uint32_t index)
{
  m_index = index;
}

ForwardingInformation::Iterator::Iterator (ForwardingInformation *policy, uint32_t index)
  : m_policy (policy),
    m_index (index)
{
}

ForwardingInformation::Iterator::~Iterator ()
{
}

ForwardingInformation::Iterator&
ForwardingInformation::Iterator::operator=(const ForwardingInformation::Iterator& iter)
{
  m_policy = iter.m_policy;
  m_index = iter.m_index
  return(*this);
}

bool
ForwardingInformation::Iterator::operator==(const ForwardingInformation::Iterator& iter)
{
  return(m_index == iter.m_index);
}

bool
ForwardingInformation::Iterator::operator!=(const ForwardingInformation::Iterator& iter)
{
  return(m_index != iter.m_index);
}

ForwardingInformation::Iterator& 
ForwardingInformation::Iterator::operator++ ()
{
  m_index++;
  return (*this);
}

ForwardingInformation::Iterator 
ForwardingInformation::Iterator::operator++ (int)
{
  NhlfeSelectionPolicy::Iterator tmp(*this);
  m_index++;
  return tmp;
}

Nhlfe&
ForwardingInformation::Iterator::operator*()
{
   return m_policy->GetNhlfe (m_index);
}

ForwardingInformation::Iterator
ForwardingInformation::Begin (void) const
{
  return m_nhlfe.begin ();
}

ForwardingInformation::Iterator
ForwardingInformation::End (void) const
{
  return m_nhlfe.end ();
}

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformation>& info)
{
  info->Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
