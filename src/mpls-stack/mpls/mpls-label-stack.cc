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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "mpls-label-stack.h"

namespace ns3 {
namespace mpls {

/// MplsLabelStack

NS_OBJECT_ENSURE_REGISTERED (MplsLabelStack);

TypeId
MplsLabelStack::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsLabelStack")
    .SetParent<Header> ()
    .AddConstructor<MplsLabelStack> ()
  ;
  return tid;
}

TypeId
MplsLabelStack::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

MplsLabelStack::MplsLabelStack ()
{
}

MplsLabelStack::~MplsLabelStack ()
{
  m_entries.clear ();
}

void
MplsLabelStack::Push (Shim shim)
{
  NS_ASSERT_MSG (!(shim & 0x100), 
                  "mpls::MplsLabelStack::Push(): Bottom Of Stack flag is set");
      
  if (!m_entries.size ())
    {
      // set Bottom Of Stack flag
      shim = (shim & 0xfffffeff) | 0x100;
    }

  m_entries.push_back (shim);
}

void
MplsLabelStack::Pop (void)
{
  m_entries.pop_back ();
}

bool
MplsLabelStack::IsEmpty (void) const
{
  return m_entries.empty ();
}

Shim
MplsLabelStack::Peek (void) const
{
  return m_entries.back ();
}

uint32_t
MplsLabelStack::GetSerializedSize (void) const
{
  return m_entries.size () * 4;
}

void
MplsLabelStack::Serialize (Buffer::Iterator start) const
{
  NS_ASSERT_MSG (m_entries.size(), 
                  "mpls::MplsLabelStack::Serialize(): stack is empty");

  for (Stack::const_reverse_iterator i = m_entries.rbegin (); i != m_entries.rend (); ++i)
    {
      start.WriteHtonU32 (*i);
    }
}

uint32_t
MplsLabelStack::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint32_t size = start.GetSize ();
  Shim shim;

  while (size > 0)
    {
      shim = i.ReadNtohU32 ();
      m_entries.push_front (shim);
      if (shimUtils::IsBottomOfStack (shim))
        {
          break;
        }
      size -= 4;
    }

  return i.GetDistanceFrom (start);
}

void
MplsLabelStack::Print (std::ostream &os) const
{
  for (Stack::const_iterator i = m_entries.end (); i != m_entries.begin (); --i)
    {
      os << shimUtils::AsString (*i) << " ";
    }
}


namespace shimUtils {

Shim
GetShim (Label label)
{
  NS_ASSERT_MSG (labelUtils::IsValidLabel (label), 
                  "mpls::shimUtils::GetShim(): invalid label value");
  return label << 12;
}

Shim
GetShim (Label label, uint8_t ttl)
{
  NS_ASSERT_MSG (labelUtils::IsValidLabel (label), 
                  "mpls::shimUtils::GetShim(): invalid label value");
  return (label << 12) | ttl;
}

Shim
GetShim (Label label, uint8_t ttl, uint8_t exp)
{
  NS_ASSERT_MSG (labelUtils::IsValidLabel (label), 
                  "mpls::shimUtils::GetShim(): invalid label value");
  NS_ASSERT_MSG (exp < 8,
                  "mpls::shimUtils::GetShim(): invalid 'experimental use' field value");
  return (label << 12) | ttl | (exp << 9);
}

void
SetLabel (Shim& shim, Label label)
{
  NS_ASSERT_MSG (labelUtils::IsValidLabel (label), 
                  "mpls::shimUtils::SetLabel(): invalid label value");
  shim = (shim & 0xfffff000) | label;
}

Label
GetLabel (Shim shim)
{
  return shim >> 12;
}


void 
SetExperimentalUse (Shim& shim, uint8_t exp)
{
  NS_ASSERT_MSG (exp < 8,
                  "mpls::shimUtils::setExperimentalUse(): invalid 'experimental use' field value");
  shim = (shim & 0xfffff1ff) | (exp << 9);
}

uint8_t 
GetExperimentalUse (Shim shim)
{
  return (shim >> 9) & 0x7;
}


void 
SetTimeToLive (Shim& shim, uint8_t ttl)
{
  shim = (shim & 0xffffff00) | ttl;
}

uint8_t
GetTimeToLive (Shim shim)
{
  return shim;
}


bool
IsBottomOfStack (Shim shim)
{
  return (shim >> 8) & 1;
}

std::string 
AsString (Shim shim)
{
  std::ostringstream os (std::ostringstream::out);
  
  os << GetLabel (shim) << " "
     << "(exp=" << (uint32_t)GetExperimentalUse (shim) << " "
     << "bos=" << (uint32_t)IsBottomOfStack (shim) << " "
     << "ttl=" << (uint32_t)GetTimeToLive (shim) << ")"
  ;
  return os.str();
}

} // namespace shimUtils

} // namespace mpls
} // namespace ns3
