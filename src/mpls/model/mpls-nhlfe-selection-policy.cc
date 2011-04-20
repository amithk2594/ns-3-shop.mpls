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
  return DoGet (nhlfe, index);
}

bool
NhlfeSelectionPolicy::Select (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
  const Ptr<const Interface> &interface, const Ptr<const Packet> &packet)
{
  std::cout << "***" << std::endl;
  std::cout << interface->GetDevice ()->GetQueue () << std::endl;
  
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

} // namespace mpls
} // namespace ns3
