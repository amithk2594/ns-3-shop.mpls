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

NS_LOG_COMPONENT_DEFINE ("MplsLabelStack");

namespace ns3 {
namespace mpls {

MplsLabelStackEntry::MplsLabelStackEntry ()
  : m_label (MPLS_LABEL_IMPLNULL),
    m_exp (0),
    m_bos (false),
    m_ttl (0)
{
}

MplsLabelStackEntry::~MplsLabelStackEntry ()
{
}

void
MplsLabelStackEntry::SetLabel (uint32_t label)
{
  NS_ASSERT_MSG (label <= MPLS_LABEL_MAX, "MplsLabelStackEntry::SetLabel (): invalid label value");
  m_label = label;
}

uint32_t
MplsLabelStackEntry::GetLabel (void) const
{
  return m_label;
}

void
MplsLabelStackEntry::SetExp (uint8_t exp)
{
  NS_ASSERT_MSG (exp <= 0x7, "MplsLabelStackEntry::SetExp (): invalid exp value");
  m_exp = exp;
}

uint8_t
MplsLabelStackEntry::GetExp (void) const
{
  return m_exp;
}

bool
MplsLabelStackEntry::IsBos (void) const
{
  return m_bos;
}

void
MplsLabelStackEntry::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}

uint8_t
MplsLabelStackEntry::GetTtl (void) const
{
  return m_ttl;
}

uint32_t
MplsLabelStackEntry::GetSerializedSize (void) const
{
  return 4;
}

void
MplsLabelStackEntry::Serialize (Buffer::Iterator start) const
{
  if (m_label == MPLS_LABEL_ROUTERALERT)
    {
      NS_ASSERT_MSG (!m_bos, "MplsLabelStackEntry::Serialize (): Misplaced Router Alert Label");
    }

  uint32_t shim = m_ttl;
  shim |= (m_bos << 8) & 0x00000100;
  shim |= (m_exp << 9) & 0x00000e00;
  shim |= (m_label << 12) & 0xfffff000;

  start.WriteHtonU32 (shim);
}

uint32_t
MplsLabelStackEntry::Deserialize (Buffer::Iterator start)
{
  uint32_t shim = start.ReadNtohU32 ();

  m_label = (shim & 0xfffff000) >> 12;
  m_exp   = (shim & 0x00000e00) >> 9;
  m_bos   = (shim & 0x00000100) >> 8;
  m_ttl   = (shim & 0x000000ff);

  return 4;
}

void
MplsLabelStackEntry::Print (std::ostream &os) const
{
  os << "label=" << m_label << " "
     << "exp=" << (uint32_t)m_exp << " "
     << "bos=" << (uint32_t)m_bos << " "
     << "ttl=" << (uint32_t)m_ttl
  ;
}

std::ostream& operator<< (std::ostream& os, const MplsLabelStackEntry &entry)
{
  entry.Print (os);
  return os;
}

NS_OBJECT_ENSURE_REGISTERED (MplsLabelStack);

MplsLabelStack::MplsLabelStack ()
{
}

MplsLabelStack::~MplsLabelStack ()
{
  for (MplsLabelStackEntryVector::iterator i = m_entries.begin (); i != m_entries.end (); i++)
    {
      *i = 0;
    }
  m_entries.clear ();
}

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

Ptr<MplsLabelStackEntry>
MplsLabelStack::Push (uint32_t label)
{
  Ptr<MplsLabelStackEntry> entry = Create<MplsLabelStackEntry> ();
  if (m_entries.size () == 0)
    {
      entry->m_bos = true;
    }

  entry->SetLabel (label);

  NS_LOG_DEBUG ("MplsLabelStack::Push (): " << *entry);

  m_entries.push_back (entry);
  return entry;
}

Ptr<MplsLabelStackEntry>
MplsLabelStack::Pop (void)
{
  if (m_entries.size () == 0)
    {
      return 0;
    }

  Ptr<MplsLabelStackEntry> entry = m_entries.back ();

  NS_LOG_DEBUG ("MplsLabelStack::Pop (): " << *entry);

  m_entries.pop_back ();
  return entry;
}

Ptr<MplsLabelStackEntry>
MplsLabelStack::Swap (uint32_t label)
{
  if (m_entries.size () == 0)
    {
      return 0;
    }

  Ptr<MplsLabelStackEntry> entry = m_entries.back ();
  entry->SetLabel (label);

  NS_LOG_DEBUG ("MplsLabelStack::Swap (): " << *entry);

  return entry;
}

MplsLabelStack::Iterator
MplsLabelStack::Begin (void) const
{
  return m_entries.begin (); // stack top
}

MplsLabelStack::Iterator
MplsLabelStack::End (void) const
{
  return m_entries.end (); // stack bottom
}

Ptr<MplsLabelStackEntry>
MplsLabelStack::GetEntry (uint32_t i) const
{
  NS_ASSERT_MSG (i < m_entries.size (), "MplsLabelStack::GetEntry (): entry index out of range");
  return m_entries[i];
}

Ptr<MplsLabelStackEntry>
MplsLabelStack::GetTopEntry (void) const
{
  if (m_entries.size () == 0)
    {
      return 0;
    }
  return m_entries.back ();
}

uint32_t
MplsLabelStack::GetNEntries (void) const
{
  return m_entries.size ();
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
  for (MplsLabelStackEntryVector::const_reverse_iterator i = m_entries.rbegin (); i != m_entries.rend (); ++i)
    {
      (*i)->Serialize (start);
      start.Next (4);
    }
}

uint32_t
MplsLabelStack::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint32_t size = start.GetSize ();
  while (size > 0)
    {
      Ptr<MplsLabelStackEntry> entry = Create<MplsLabelStackEntry> ();
      uint32_t deserialized = entry->Deserialize (i);
      m_entries.insert (m_entries.begin (), entry);
      i.Next (deserialized);
      if (entry->m_bos)
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
  for (MplsLabelStackEntryVector::const_iterator i = m_entries.end (); i != m_entries.begin (); --i)
    {
      (*i)->Print (os);
    }
}

} // namespace mpls
} // namespace ns3
