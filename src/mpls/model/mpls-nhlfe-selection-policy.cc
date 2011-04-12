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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/callback.h"

#include "mpls-nhlfe-selection-policy.h"

NS_LOG_COMPONENT_DEFINE ("mpls::NhlfeSelectionPolicy");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (NhlfeSelectionPolicy);

TypeId
NhlfeSelectionPolicy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::NhlfeSelectionPolicy")
    .SetParent<Object> ()
    .AddConstructor<NhlfeSelectionPolicy> ()
  ;
  return tid;
}

NhlfeSelectionPolicy::Info::Info ()
{
}

NhlfeSelectionPolicy::Info::~Info ()
{
}



NhlfeSelectionPolicy::NhlfeSelectionPolicy ()
  : m_fwd (0)
{
  NS_LOG_FUNCTION (this);
}

NhlfeSelectionPolicy::~NhlfeSelectionPolicy ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
NhlfeSelectionPolicy::DoDispose (void)
{
  m_fwd = 0;
  
  Object::DoDispose ();
}
  
Nhlfe*
NhlfeSelectionPolicy::GetNhlfe (uint32_t index)
{
  return &(m_fwd->m_nhlfe[index]);
}

uint32_t 
NhlfeSelectionPolicy::GetNNhlfe (void)
{
  return m_fwd->m_nhlfe.size ();
}
  
} // namespace mpls
} // namespace ns3
