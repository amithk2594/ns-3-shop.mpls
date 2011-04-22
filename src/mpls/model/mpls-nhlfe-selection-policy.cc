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
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include <functional>

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
    .AddAttribute ("MaxPacketsInTxQueue", 
                   "The maximum number of packets in Queue",
                   IntegerValue (-1),
                   MakeIntegerAccessor (&NhlfeSelectionPolicy::m_maxPackets),
                   MakeIntegerChecker<int32_t> ())
    .AddAttribute ("MaxBytesInTxQueue", 
                   "The maximum number of bytes in Queue.",
                   IntegerValue (-1),
                   MakeIntegerAccessor (&NhlfeSelectionPolicy::m_maxBytes),
                   MakeIntegerChecker<int32_t> ())
  ;
  return tid;
}

NhlfeSelectionPolicy::NhlfeSelectionPolicy ()
  : m_maxPackets (-1),
    m_maxBytes (-1)
{
}

NhlfeSelectionPolicy::~NhlfeSelectionPolicy ()
{
}

const Nhlfe&
NhlfeSelectionPolicy::Get (const std::vector<Nhlfe> &nhlfe, uint32_t index)
{
  if (index == 0) 
    {
        DoStart (nhlfe.size ());
    }

  return DoGet (nhlfe, index);
}

bool
NhlfeSelectionPolicy::Select (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
  const Ptr<const Interface> &interface, const Ptr<const Packet> &packet)
{
  Ptr<Queue> queue = interface->GetDevice ()->GetQueue ();
  
  if (queue != 0)
    {
      if (m_maxPackets >=0 && queue->GetNPackets () > (uint32_t)m_maxPackets)
        {
          return false;
        }
      if (m_maxBytes >=0 && queue->GetNBytes () > (uint32_t)m_maxBytes)
        {
          return false;
        }
    }
    
  return DoSelect (nhlfe, index, interface, packet);
}

void
NhlfeSelectionPolicy::DoStart (uint32_t size)
{
}

const Nhlfe&
NhlfeSelectionPolicy::DoGet (const std::vector<Nhlfe> &nhlfe, uint32_t index)
{
  return nhlfe[index];
}

bool
NhlfeSelectionPolicy::DoSelect (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
  const Ptr<const Interface> &interface, const Ptr<const Packet> &packet)
{
  return true;
}

void
NhlfeSelectionPolicy::Print (std::ostream &os) const
{
  os << "default policy";
}


TypeId
RoundRobinPolicy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::RoundRobinPolicy")
    .SetParent<NhlfeSelectionPolicy> ()
    .AddConstructor<RoundRobinPolicy> ()
  ;
  return tid;
}

RoundRobinPolicy::RoundRobinPolicy ()
  : m_index (0)
{
}

RoundRobinPolicy::~RoundRobinPolicy ()
{
}

const Nhlfe&
RoundRobinPolicy::DoGet (const std::vector<Nhlfe>& nhlfe, uint32_t index)
{
  index = m_index++;

  if (nhlfe.size () >= m_index)
    {
      m_index = 0;
    }

  return nhlfe[index];
}

void
RoundRobinPolicy::Print (std::ostream& os) const
{
  os << "round robin policy";
}

TypeId
StaRoundRobinPolicy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::StaRoundRobinPolicy")
    .SetParent<NhlfeSelectionPolicy> ()
    .AddConstructor<StaRoundRobinPolicy> ()
  ;
  return tid;
}

StaRoundRobinPolicy::StaRoundRobinPolicy ()
  : m_mapping ()
{
  m_iter = m_mapping.begin ();
}

StaRoundRobinPolicy::~StaRoundRobinPolicy ()
{
}

void
StaRoundRobinPolicy::DoStart (uint32_t size)
{
  if (m_mapping.size () != size)
    {
      m_mapping.resize (size);
      uint32_t idx = 0;
      for (std::list<uint32_t>::iterator i = m_mapping.begin (); i != m_mapping.end (); ++i, ++idx)
        {
          *i = idx;
        }
    }
  m_iter = m_mapping.begin ();    
}

const Nhlfe&
StaRoundRobinPolicy::DoGet (const std::vector<Nhlfe>& nhlfe, uint32_t index)
{
  index = *m_iter;
  
  ++m_iter;
  
  if (m_iter == m_mapping.end ())
    {
      m_iter = m_mapping.begin ();
    }

  return nhlfe[index];
}

bool 
StaRoundRobinPolicy::DoSelect (const std::vector<Nhlfe>& nhlfe, uint32_t index,
  const Ptr<const Interface>& interface, const Ptr<const Packet>& packet)
{
  if (m_iter != m_mapping.begin ())
    {
      --m_iter;
      m_mapping.push_back (*m_iter);
      m_mapping.erase (m_iter);
    }

  return true;
}

void
StaRoundRobinPolicy::Print (std::ostream& os) const
{
  os << "sta round robin policy";
}


TypeId
WeightedSelectionPolicy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::WeightedSelectionPolicy")
    .SetParent<NhlfeSelectionPolicy> ()
    .AddConstructor<WeightedSelectionPolicy> () 
    .AddAttribute ("Bmin", 
                   "The minimum number of bytes of the byte counter.",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&WeightedSelectionPolicy::m_Bmin),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Bmax", 
                   "The maximum number of bytes of the byte counter.",
                   UintegerValue (1000000),
                   MakeUintegerAccessor (&WeightedSelectionPolicy::m_Bmax),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

WeightedSelectionPolicy::WeightedSelectionPolicy ()
  : m_Btot (0),
    m_Bmin (1000),
    m_Bmax (1000000),
    m_weights (),
    m_mapping ()
{
}

WeightedSelectionPolicy::~WeightedSelectionPolicy ()
{
}

void
WeightedSelectionPolicy::DoStart (uint32_t size)
{
  if (m_mapping.size () != size)
    {
      m_mapping.resize (size);
      uint32_t idx = 0;
      for (std::list<NhlfeInfo>::iterator i = m_mapping.begin (); i != m_mapping.end (); ++i, ++idx)
        {
          i->m_index = idx;
	  i->m_currentRatio = 0.0;
	  i->m_requiredRatio = (idx < m_weights.size () ? m_weights[idx] : 0.0);
        }
    }
  m_iter = m_mapping.begin ();    
}

const Nhlfe&
WeightedSelectionPolicy::DoGet (const std::vector< Nhlfe >& nhlfe, uint32_t index)
{
    return nhlfe[m_iter->m_index];
}

bool WeightedSelectionPolicy::DoSelect(const std::vector< Nhlfe >& nhlfe, uint32_t index,
  const Ptr< const Interface >& interface, const Ptr< const Packet >& packet)
{
  for (std::list<NhlfeInfo>::iterator i = m_mapping.begin (); i != m_mapping.end (); ++i)
  {
    if (i != m_iter)
      i->m_currentRatio *= m_Btot / (m_Btot + packet->GetSize ());
    else
      i->m_currentRatio = (i->m_currentRatio * m_Btot + packet->GetSize ()) / (m_Btot + packet->GetSize ());
  }
  
  m_Btot += packet->GetSize ();

  if (m_Btot > m_Bmax)
    m_Btot = m_Bmin;
  
  m_mapping.sort (std::mem_fun_ref (&NhlfeInfo::DecreasingDiffOrder));
  
  return true;
}


void
WeightedSelectionPolicy::Print (std::ostream& os) const
{
  os << "weighted selection policy";
}

void 
WeightedSelectionPolicy::SetWeights (const std::vector<double>& weights)
{
  m_weights = weights;
}

bool
WeightedSelectionPolicy::NhlfeInfo::DecreasingDiffOrder (const WeightedSelectionPolicy::NhlfeInfo& y) const
{
  return m_requiredRatio - m_currentRatio > y.m_requiredRatio - y.m_currentRatio;
}


} // namespace mpls
} // namespace ns3
