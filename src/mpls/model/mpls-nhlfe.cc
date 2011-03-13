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
#include "ns3/ipv4-address.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

Nhlfe::Nhlfe (const Operation& op, int32_t outInterface)
  : m_interface (outInterface)
{
  NS_ASSERT_MSG (outInterface, "Invalid outgoing interface index");
  op.Accept (*this);
}

Nhlfe::Nhlfe (const Operation& op, const Address& nextHop)
  : m_interface (-1),
    m_nextHop (nextHop)
{
  NS_ASSERT_MSG (!nextHop.IsInvalid (), "Invalid next-hop address");
  op.Accept (*this);
}

Nhlfe::Nhlfe (const Operation& op)
  : m_interface (-1)
{
  op.Accept (*this);
}

Nhlfe::~Nhlfe ()
{
}

int32_t
Nhlfe::GetInterface (void) const
{
  return m_interface;
}

const Address& 
Nhlfe::GetNextHop (void) const
{
  return m_nextHop;
}

uint32_t 
Nhlfe::GetOpCode (void) const
{
  return m_opcode;
}

uint32_t 
Nhlfe::GetNLabels (void) const
{
  return m_count;
}

uint32_t 
Nhlfe::GetLabel (uint32_t index) const
{
  return m_labels[index];
}
  
void
Nhlfe::Print (std::ostream &os) const
{
  switch (m_opcode) 
    {
      case OP_POP:
        os << "pop";
        break;
      case OP_SWAP:
        os << "swap";
        for (uint32_t i = 0; i < m_count; ++i)
          {
            os << "," << Label (m_labels[i]);
          }
        break;
    }

  os << " ";
  
  if (m_interface >= 0)
    {
      os << "oif " << m_interface;
    }
  else 
    {
      os << "nexthop ";
      if (Ipv4Address::IsMatchingType (m_nextHop))
        {
          os << Ipv4Address::ConvertFrom (m_nextHop);
        }
      else
        {
          os << m_nextHop;
        }
    }  
}

std::ostream& operator<< (std::ostream& os, const Nhlfe& nhlfe)
{
  nhlfe.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
