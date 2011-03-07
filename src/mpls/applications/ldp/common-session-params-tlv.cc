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

#include "common-session-params-tlv.h"

namespace ns3 {
namespace ldp {

ENSURE_REGISTER_TLV (CommonSessionParamsTLV);

const uint16_t CommonSessionParamsTLV::TLV_TYPE = 0x0500;

Ptr<TypeLengthValue>
CommonSessionParamsTLV::Factory (void)
{
  return Create<CommonSessionParamsTLV> ();
}

CommonSessionParamsTLV::CommonSessionParamsTLV ()
  : m_keepAliveTime (0),
    m_downstreamOnDemand (false),
    m_loopDetection (false),
    m_pathVectorLimit (0),
    m_ldpIdentifier (0)
{
}

CommonSessionParamsTLV::~CommonSessionParamsTLV ()
{
}

void
CommonSessionParamsTLV::SetKeepAliveTime (uint16_t time)
{
  m_keepAliveTime = time;
}

uint16_t
CommonSessionParamsTLV::GetKeepAliveTime (void) const
{
  return m_keepAliveTime;
}

void
CommonSessionParamsTLV::SetDownstreamOnDemand (bool enabled)
{
  m_downstreamOnDemand = enabled;
}

bool
CommonSessionParamsTLV::IsDownstreamOnDemand (void) const
{
  return m_downstreamOnDemand;
}

void
CommonSessionParamsTLV::SetLoopDetection (bool enabled)
{
  m_loopDetection = enabled;
}

bool
CommonSessionParamsTLV::IsLoopDetection (void) const
{
  return m_loopDetection;
}

void
CommonSessionParamsTLV::SetPathVectorLimit (uint8_t limit)
{
  m_pathVectorLimit = limit;
}

uint8_t
CommonSessionParamsTLV::GetPathVectorLimit (void) const
{
  return m_pathVectorLimit;
}

void
CommonSessionParamsTLV::SetMaxPduLength (uint16_t length)
{
  m_maxPduLength = length;
}

uint16_t
CommonSessionParamsTLV::GetMaxPduLength (void) const
{
  return m_maxPduLength;
}

void
CommonSessionParamsTLV::SetLdpIdentifier (uint32_t id)
{
  m_ldpIdentifier = id;
}

uint32_t
CommonSessionParamsTLV::GetLdpIdentifier (void) const
{
  return m_ldpIdentifier;
}

uint16_t
CommonSessionParamsTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
CommonSessionParamsTLV::GetLength (void) const
{
  return 14;
}

void
CommonSessionParamsTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteHtonU16 (1); // Protocol version
  start.WriteHtonU16 (m_keepAliveTime);
  uint8_t adBits = 0;
  if (m_downstreamOnDemand)
    {
      adBits |= 0x80;
    }
  if (m_loopDetection)
    {
      adBits |= 0x40;
    }
  start.WriteU8 (adBits);
  start.WriteU8 (m_pathVectorLimit);
  start.WriteHtonU16 (m_maxPduLength);
  start.WriteHtonU32 (m_ldpIdentifier);
  start.WriteU16 (0);
}

bool
CommonSessionParamsTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 14)
    {
      return false;
    }

  if (start.ReadNtohU16 () != 1)
    {
      return false;
    }

  m_keepAliveTime = start.ReadNtohU16 ();
  uint8_t adBits = start.ReadU8 ();
  m_downstreamOnDemand = (adBits & 0x80) > 0;
  m_loopDetection = (adBits & 0x40) > 0;
  m_pathVectorLimit = start.ReadU8 ();
  m_maxPduLength = start.ReadNtohU16 ();
  m_ldpIdentifier = start.ReadNtohU32 ();

  return true;
}

void
CommonSessionParamsTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Common Session Params");
  os << "KeepAlive=" << (uint32_t)m_keepAliveTime << " ";
  if (m_downstreamOnDemand)
    {
      os << "A ";
    }
  if (m_loopDetection)
    {
      os << "D ";
    }
  os << "PVLim=" << (uint32_t)m_pathVectorLimit
     << " MaxPDU=" << m_maxPduLength
     << " LdpId=" << std::hex << m_ldpIdentifier << std::dec;
}

} // namespace ldp
} // namespace ns3
