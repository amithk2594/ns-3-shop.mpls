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
                   UintegerValue (0),
                   MakeUintegerAccessor (&NhlfeSelectionPolicy::m_maxPackets),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxBytesInTxQueue", 
                   "The maximum number of bytes in Queue.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&NhlfeSelectionPolicy::m_maxBytes),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

NhlfeSelectionPolicy::NhlfeSelectionPolicy ()
  : m_maxPackets (0),
    m_maxBytes (0)
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
      if (m_maxPackets && queue->GetNPackets () > m_maxPackets)
        {
          return false;
        }
      if (m_maxBytes && queue->GetNBytes () > m_maxBytes)
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
      m_iter = m_mapping.begin ()
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
    .SetParent<NhlfeSelectionPolicy()> ()
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
{
}

WeightedSelectionPolicy::~WeightedSelectionPolicy ()
{
}

void
WeightedSelectionPolicy::Print (std::ostream& os) const
{
  os << "weighted selection policy";
}

} // namespace mpls
} // namespace ns3
