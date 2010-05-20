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

#include "common-hello-params-tlv.h"

namespace ns3 {
namespace ldp {

ENSURE_REGISTER_TLV (CommonHelloParamsTLV);

const uint16_t CommonHelloParamsTLV::TLV_TYPE = 0x0400;

Ptr<TypeLengthValue>
CommonHelloParamsTLV::Factory (void)
{
  return Create<CommonHelloParamsTLV> ();
}

CommonHelloParamsTLV::CommonHelloParamsTLV ()
  : m_holdTime (0)
{
}

CommonHelloParamsTLV::~CommonHelloParamsTLV ()
{
}

uint16_t
CommonHelloParamsTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
CommonHelloParamsTLV::GetLength (void) const
{
  return 4;
}

void
CommonHelloParamsTLV::SetHoldTime (uint16_t time)
{
  m_holdTime = time;
}

uint16_t
CommonHelloParamsTLV::GetHoldTime (void) const
{
  return m_holdTime;
}

void
CommonHelloParamsTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteHtonU16 (m_holdTime);
  start.WriteHtonU16 (0);
}

bool
CommonHelloParamsTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 4)
    {
      return false;
    }

  m_holdTime = start.ReadNtohU16 ();
  return true;
}

void
CommonHelloParamsTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Common Hello Params");
  os << "Hold Time = " << (uint32_t)m_holdTime;
}

} // namespace ldp
} // namespace ns3
