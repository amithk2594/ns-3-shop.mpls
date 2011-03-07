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
#include "label-request-message.h"

namespace ns3 {
namespace ldp {

LabelRequestMessage::LabelRequestMessage ()
  : m_fec (0),
    m_hopCount (0),
    m_pathVector (0)
{
}

LabelRequestMessage::~LabelRequestMessage ()
{
  m_fec = 0;
  m_hopCount = 0;
  m_pathVector = 0;
}

void
LabelRequestMessage::SetFec (Ptr<FecTLV> tlv)
{
  m_fec = fec;
}

Ptr<FecTLV>
LabelRequestMessage::GetFec (void) const
{
  return m_fec;
}

void
LabelRequestMessage::SetHopCount (Ptr<HopCountTLV> hopCount)
{
  m_hopCount = hopCount;
}

Ptr<HopCountTLV>
LabelRequestMessage::GetHopCount (void) const
{
  return m_hopCount;
}

void
LabelRequestMessage::SetPathVector (Ptr<PathVectorTLV> pathVector)
{
  m_pathVector = pathVector;
}

Ptr<PathVectorTLV>
LabelRequestMessage::GetPathVector (void) const
{
  return m_pathVector;
}

uint16_t
LabelRequestMessage::GetType (void) const
{
  return MESSAGE_TYPE;
}

uint32_t
LabelRequestMessage::GetLength (void) const
{
  uint32_t length = 0;

  if (m_fec != 0)
    {
      length += m_fec->GetLength ();
    }

  if (m_hopCount != 0)
    {
      length += m_hopCount->GetLength ();
    }

  if (m_pathVector != 0)
    {
      length += m_pathVector->GetLength ();
    }

  return length;
}

void
LabelRequestMessage::SerializeMessage (Buffer::Iterator start) const
{
  if (m_fec != 0)
    {
      m_fec->Serialize (start);
    }

  if (m_hopCount != 0)
    {
      m_hopCount->Serialize (start);
    }

  if (m_pathVector != 0)
    {
      m_pathVector->Serialize (start);
    }
}

bool
LabelRequestMessage::DeserializeMessage (Buffer::Iterator &start, uint32_t size, enum PduDecodingErrno &errno)
{
  if (TypeLengthValue::IsMatchingType (start, FecTLV::FEC_TYPE))
    {
      m_fec = Create<FecTLV> ();
      if (!TypeLengthValue::Deserialize (start, m_fec, size, errno))
        {
          return errno;
        }
    }

  while (size > 0)
    {
      if (TypeLengthValue::IsMatchingType (i, HopCountTLV::FEC_TYPE))
        {
          m_hopCount = Create<HopCountTLV> ();
          if (!TypeLengthValue::Deserialize (i, m_hopCount, size, errno))
            {
              return errno;
            }
        }
      else if (TypeLengthValue::IsMatchingType (i, PathVectorTLV::FEC_TYPE))
        {
          m_pathVector = Create<PathVectorTLV> ();
          if (!TypeLengthValue::Deserialize (i, m_pathVector, size, errno))
            {
              return errno;
            }
        }
      else
        {
          if (!HandleUnknownTlv (i, size, errno))
            {
              return errno;
            }
        }
    }

  return true;
}

void
LabelRequestMessage::Print (std::ostream &os) const
{
  PrintMessage (os, "Label Request Message");
}

} // namespace ldp
} // namespace ns3
