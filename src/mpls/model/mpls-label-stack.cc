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
#include "mpls-label-stack.h"

NS_LOG_COMPONENT_DEFINE ("mpls::LabelStack");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (LabelStack);

TypeId
LabelStack::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::LabelStack")
    .SetParent<Header> ()
    .AddConstructor<LabelStack> ()
  ;
  return tid;
}

TypeId
LabelStack::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

LabelStack::LabelStack ()
  : m_broken (false)
{
}

LabelStack::~LabelStack ()
{
  m_entries.clear ();
}

void
LabelStack::Push (uint32_t s)
{
  NS_LOG_FUNCTION (this << s);

  m_entries.push_back (s);
}

void
LabelStack::Swap (uint32_t s)
{
  NS_LOG_FUNCTION (this << s);

  if (m_entries.empty ())
    {
      m_entries.push_back (s);
    }
  else 
    {
      m_entries.back () = s;
    }
}

void
LabelStack::Pop (void)
{
  m_entries.pop_back ();
}

bool
LabelStack::IsEmpty (void) const
{
  return m_entries.empty ();
}

uint32_t&
LabelStack::Peek (void)
{
  return m_entries.back ();
}

uint32_t
LabelStack::Peek (void) const
{
  return m_entries.back ();
}

bool
LabelStack::IsBroken (void) const
{
  return m_broken;
}

uint32_t
LabelStack::GetSerializedSize (void) const
{
  return m_entries.size () * 4;
}

void
LabelStack::Serialize (Buffer::Iterator start) const
{
  NS_ASSERT_MSG (m_entries.size(), "Empty label stack");

  Stack::const_reverse_iterator i = m_entries.rbegin ();
  Stack::const_reverse_iterator end = m_entries.rend () - 1;

  while (i != end)
    {
      start.WriteHtonU32 (*i++);
    }

  start.WriteHtonU32 (shim::SetBos (m_entries[0]));
}

uint32_t
LabelStack::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint32_t size = start.GetSize ();
  uint32_t s;
  
  m_broken = true;

  while (size > 0)
    {
      s = i.ReadNtohU32 ();
      if (shim::IsBos (s))
        {
          m_entries.push_front (shim::ClearBos (s));
          m_broken = false;
          break;
        }
      else
        {
          m_entries.push_front (s);
        }
      size -= 4;
    }

  return i.GetDistanceFrom (start);
}

void
LabelStack::Print (std::ostream &os) const
{
  if (m_entries.size ()) 
    {
      os << Label (shim::GetLabel (m_entries[0]));
      
      for (Stack::const_iterator i = m_entries.begin () + 1;
           i != m_entries.end ();
           i++)
        {
          os << " " << Label (shim::GetLabel (*i));
        }
    }
  else
    {
      os << "empty";
    }
}

} // namespace mpls
} // namespace ns3
