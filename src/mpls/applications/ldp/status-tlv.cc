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

#include "status-tlv.h"

namespace ns3 {
namespace ldp {

ENSURE_REGISTER_TLV (StatusTLV);

const uint16_t StatusTLV::TLV_TYPE = 0x0300;

Ptr<TypeLengthValue>
StatusTLV::Factory (void)
{
  return Create<StatusTLV> ();
}

StatusTLV::StatusTLV ()
  : m_statusCode (0),
    m_messageId (0),
    m_messageType (0)
{
}

StatusTLV::~StatusTLV ()
{
}

void
StatusTLV::SetStatusCode (uint32_t statusCode)
{
  m_statusCode = statusCode;
}

uint32_t
StatusTLV::GetStatusCode (void) const
{
  return m_statusCode;
}

bool
StatusTLV::IsFatalError (void) const
{
  return m_statusCode & 0x80000000;
}

void
StatusTLV::SetMessageId (uint32_t messageId)
{
  m_messageId = messageId;
}

uint32_t
StatusTLV::GetMessageId (void) const
{
  return m_messageId;
}

void
StatusTLV::SetMessageType (uint16_t messageType)
{
  m_messageType = messageType;
}

uint16_t
StatusTLV::GetMessageType (void) const
{
  return m_messageType;
}

uint16_t
StatusTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
StatusTLV::GetLength (void) const
{
  return 10;
}

void
StatusTLV::SerializeValue (Buffer::Iterator &start) const
{
  uint32_t status = m_statusCode;
  if (IsForward ())
    {
      status |= 0x40000000;
    }

  start.WriteHtonU32 (status);
  start.WriteHtonU32 (m_messageId);
  start.WriteHtonU16 (m_messageType);
}

bool
StatusTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 10)
    {
      return false;
    }

  m_statusCode = start.ReadNtohU32 () & 0xbfffffff;
  m_messageId = start.ReadNtohU32 ();
  m_messageType = start.ReadNtohU16 ();

  return true;
}

void
StatusTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Status TLV");
  if (IsFatalError ())
    {
      os << "E ";
    }
  if (IsForward ())
    {
      os << "F ";
    }

  os << "Code=" << std::hex << m_statusCode << " "
     << "Message Id" << std::hex << m_messageId << " "
     << "Message Type" << std::hex << m_messageType
  ;
}

} // namespace ldp
} // namespace ns3
