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

#include <iomanip>

#include "ns3/assert.h"

#include "mpls-lib-entry.h"

namespace ns3 {
namespace mpls {

MplsLibEntry::MplsLibEntry ()
  : m_fec (0),
    m_inIfIndex (-1),
    m_inLabel (-1),
    m_outIfIndex (-1),
    m_operation (SWAP),
    m_outLabel (-1)
{
}

MplsLibEntry::~MplsLibEntry ()
{
  delete m_fec;
}

uint32_t
MplsLibEntry::GetInLabel (void) const
{
  return m_inLabel;
}

void
MplsLibEntry::SetInLabel (uint32_t label)
{
  m_inLabel = label;
}

uint32_t
MplsLibEntry::GetOutLabel (void) const
{
  return m_outLabel;
}

void
MplsLibEntry::SetOutLabel (uint32_t label)
{
  m_outLabel = label;
}

int32_t
MplsLibEntry::GetInIfIndex (void) const
{
  return m_inIfIndex;
}

void
MplsLibEntry::SetInIfIndex (int32_t ifIndex)
{
  m_inIfIndex = ifIndex;
}

int32_t
MplsLibEntry::GetOutIfIndex (void) const
{
  return m_outIfIndex;
}

void
MplsLibEntry::SetOutIfIndex (int32_t ifIndex)
{
  m_outIfIndex = ifIndex;
}

enum MplsLibEntry::LabelOperation
MplsLibEntry::GetOperation (void) const
{
  return m_operation;
}

void
MplsLibEntry::SetOperation (enum MplsLibEntry::LabelOperation operation)
{
  m_operation = operation;
}

const ForwardingEquivalenceClass*
MplsLibEntry::GetFec (void) const
{
  return m_fec;
}

void
MplsLibEntry::SetFec (const ForwardingEquivalenceClass &fec)
{
  delete m_fec;
  m_fec = fec.Copy ();
}

void
MplsLibEntry::Print (std::ostream &os) const
{
  os << std::setiosflags (std::ios::left);
  os << std::setw (6);
  if (m_inIfIndex == -1)
    {
      os << " ";
    }
  else
    {
      os << m_inIfIndex;
    }

  os << std::setw (30);
  if (m_fec != 0)
    {
      m_fec->Print (os);
    }
  else
    {
      os << m_inLabel;
    }

  os << std::setw (6);
  if (m_outIfIndex == -1)
    {
      os << " ";
    }
  else
    {
      os << m_outIfIndex;
    }

  os << std::setw (6);

  switch (m_operation)
    {
    case PUSH:
      os << "PUSH";
      break;

    case POP:
      os << "POP";
      break;

    case SWAP:
      os << "SWAP";
      break;
    }

  os << std::setw (9);
  if (m_outLabel == uint32_t(-1) || m_operation == POP)
    {
      os << " ";
    }
  else
    {
      os << m_outLabel;
    }
}

std::ostream& operator<< (std::ostream& os, const MplsLibEntry &entry)
{
  entry.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
