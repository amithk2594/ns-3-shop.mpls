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

namespace ns3 {
namespace mpls {

MplsLabelEntry::MplsLabelEntry ()
  : m_label (-1),
    m_exp (0),
    m_bos (false),
    m_ttl (0)
{
}

MplsLabelEntry::MplsLabelEntry (const MplsLabel &label)
  : m_label (label),
    m_exp (0),
    m_bos (false),
    m_ttl (0)
{
}

MplsLabelEntry::~MplsLabelEntry ()
{
}

void
MplsLabelEntry::SetLabel (const MplsLabel &label)
{
  m_label = label;
}

const MplsLabel&
MplsLabelEntry::GetLabel (void) const
{
  return m_label;
}

void
MplsLabelEntry::SetExp (uint8_t exp)
{
  NS_ASSERT_MSG (exp <= 0x7, "MplsLabelEntry::SetExp (): invalid exp value");
  m_exp = exp;
}

uint8_t
MplsLabelEntry::GetExp (void) const
{
  return m_exp;
}

bool
MplsLabelEntry::IsBos (void) const
{
  return m_bos;
}

void
MplsLabelEntry::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}

uint8_t
MplsLabelEntry::GetTtl (void) const
{
  return m_ttl;
}

uint32_t
MplsLabelEntry::GetSerializedSize (void) const
{
  return 4;
}

void
MplsLabelEntry::Serialize (Buffer::Iterator start) const
{
  if (m_label.IsRouteAlert ())
    {
      NS_ASSERT_MSG (!m_bos, "MplsLabelEntry::Serialize (): Misplaced Router Alert label");
    }

  uint32_t shim = m_ttl;
  shim |= (m_bos << 8) & 0x00000100;
  shim |= (m_exp << 9) & 0x00000e00;
  shim |= (m_label.GetValue () << 12) & 0xfffff000;

  start.WriteHtonU32 (shim);
}

uint32_t
MplsLabelEntry::Deserialize (Buffer::Iterator start)
{
  uint32_t shim = start.ReadNtohU32 ();

  m_label = (shim & 0xfffff000) >> 12;
  m_exp   = (shim & 0x00000e00) >> 9;
  m_bos   = (shim & 0x00000100) >> 8;
  m_ttl   = (shim & 0x000000ff);

  return 4;
}

void
MplsLabelEntry::Print (std::ostream &os) const
{
  os << m_label << " "
     << "(exp=" << (uint32_t)m_exp << " "
     << "bos=" << (uint32_t)m_bos << " "
     << "ttl=" << (uint32_t)m_ttl << ")"
  ;
}

std::ostream& operator<< (std::ostream& os, const MplsLabelEntry &entry)
{
  entry.Print (os);
  return os;
}

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
MplsLabelStack::Push (const MplsLabelEntry& entry)
{
  entry.m_bos = m_entries.size () == 0;
  m_entries.push_back (entry);
}

void
MplsLabelStack::Pop (void)
{
  m_entries.pop_back ();
}

bool
MplsLabelStack::IsEmpty (void) const
{
  return m_enties.empty ();
}

MplsLabelEntry&
MplsLabelStack::GetTop (void)
{
  return m_entries.back ();
}

const MplsLabelEntry&
MplsLabelStack::GetTop (void) const
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
  // The top of the label stack appears earliest in the packet,
  // and the bottom appears latest.
  for (MplsLabelEntryVector::const_reverse_iterator i = m_entries.rbegin (); i != m_entries.rend (); ++i)
    {
      (*i).Serialize (start);
      start.Next (4);
    }
}

uint32_t
MplsLabelStack::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint32_t size = start.GetSize ();
  MplsLabelEntry entry;

  while (size > 0)
    {
      uint32_t deserialized = entry.Deserialize (i);
      m_entries.insert (m_entries.begin (), entry);
      i.Next (deserialized);

      if (entry.m_bos)
        {
          break;
        }

      size -= deserialized;
    }

  return i.GetDistanceFrom (start);
}

void
MplsLabelStack::Print (std::ostream &os) const
{
  for (MplsLabelEntryVector::const_iterator i = m_entries.end (); i != m_entries.begin (); --i)
    {
      (*i).Print (os);
      os >> " ";
    }
}

} // namespace mpls
} // namespace ns3
