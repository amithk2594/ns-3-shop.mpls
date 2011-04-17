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

#include "common-tlv.h"

namespace ns3 {
namespace ldp {

/**
 * Hop Count TLV
 */

ENSURE_REGISTER_TLV (HopCountTLV);

const uint16_t HopCountTLV::TLV_TYPE = 0x0103;

Ptr<TypeLengthValue>
HopCountTLV::Factory (void)
{
  return Create<HopCountTLV> ();
}

HopCountTLV::HopCountTLV ()
  : m_value (0)
{
}

HopCountTLV::HopCountTLV (uint8_t value)
  : m_value (value)
{
}

HopCountTLV::~HopCountTLV ()
{
}

uint16_t
HopCountTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
HopCountTLV::GetLength (void) const
{
  return 1;
}

void
HopCountTLV::SetValue (uint8_t value)
{
  m_value = value;
}

uint8_t
HopCountTLV::GetValue (void) const
{
  return m_value;
}

void
HopCountTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteU8 (m_value);
}

bool
HopCountTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 1)
    {
      return false;
    }

  m_value = start.ReadU8 ();
  return true;
}

void
HopCountTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Hop Count TLV");
  os << "value=" << std::hex << m_value << std::dec;
}

/**
 * Generic Label TLV
 */

ENSURE_REGISTER_TLV (GenericLabelTLV);

const uint16_t GenericLabelTLV::TLV_TYPE = 0x0200;

Ptr<TypeLengthValue>
GenericLabelTLV::Factory (void)
{
  return Create<GenericLabelTLV> ();
}

GenericLabelTLV::GenericLabelTLV ()
  : m_label (3) // Implicit NULL Label
{
}

GenericLabelTLV::GenericLabelTLV (uint32_t label)
{
  SetLabel (label);
}

GenericLabelTLV::~GenericLabelTLV ()
{
}

uint16_t
GenericLabelTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
GenericLabelTLV::GetLength (void) const
{
  return 4;
}

void
GenericLabelTLV::SetLabel (uint32_t label)
{
  NS_ASSERT_MSG (label <= 0xfffff, "GenericLabelTLV::SetLabel (): invalid label value");
  m_label = label;
}

uint32_t
GenericLabelTLV::GetLabel (void) const
{
  return m_label;
}

void
GenericLabelTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteHtonU32 (m_label);
}

bool
GenericLabelTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 4)
    {
      return false;
    }

  m_label = start.ReadNtohU32 ();
  return m_label < 0xfffff;
}

void
GenericLabelTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Generic Label TLV ");
  os << "value=" << std::hex << m_label << std::dec;
}

/**
 * Message ID TLV
 */

ENSURE_REGISTER_TLV (MessageIdTLV);

const uint16_t MessageIdTLV::TLV_TYPE = 0x0600;

Ptr<TypeLengthValue>
MessageIdTLV::Factory (void)
{
  return Create<MessageIdTLV> ();
}

MessageIdTLV::MessageIdTLV ()
  : m_msgid (0) // Implicit NULL Label
{
}

MessageIdTLV::MessageIdTLV (uint32_t msgid)
{
  m_msgid = msgid;
}

MessageIdTLV::~MessageIdTLV ()
{
}

uint16_t
MessageIdTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
MessageIdTLV::GetLength (void) const
{
  return 4;
}

void
MessageIdTLV::SetMessageId (uint32_t msgid)
{
  m_msgid = msgid;
}

uint32_t
MessageIdTLV::GetMessageId (void) const
{
  return m_msgid;
}

void
MessageIdTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteHtonU32 (m_msgid);
}

bool
MessageIdTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 4)
    {
      return false;
    }

  m_msgid = start.ReadNtohU32 ();
  return true;
}

void
MessageIdTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Message ID TLV ");
  os << "id=" << std::hex << m_msgid << std::dec;
}

/**
 * Ipv4 Transport Address
 */
ENSURE_REGISTER_TLV (Ipv4TransportAddressTLV);

const uint16_t Ipv4TransportAddressTLV::TLV_TYPE = 0x0401;

Ptr<TypeLengthValue>
Ipv4TransportAddressTLV::Factory (void)
{
  return Create<Ipv4TransportAddressTLV> ();
}

Ipv4TransportAddressTLV::Ipv4TransportAddressTLV ()
  : m_address (Ipv4Address::GetZero())
{
}

Ipv4TransportAddressTLV::Ipv4TransportAddressTLV (const Ipv4Address &address)
  : m_address (address)
{
}

Ipv4TransportAddressTLV::~Ipv4TransportAddressTLV ()
{
}

uint16_t
Ipv4TransportAddressTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
Ipv4TransportAddressTLV::GetLength (void) const
{
  return 4;
}

void
Ipv4TransportAddressTLV::SetAddress (const Ipv4Address &address)
{
  m_address = address;
}

Ipv4Address
Ipv4TransportAddressTLV::GetAddress (void) const
{
  return m_address;
}

void
Ipv4TransportAddressTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.WriteHtonU32 (m_address.Get ());
}

bool
Ipv4TransportAddressTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 4)
    {
      return false;
    }

  m_address.Set (start.ReadNtohU32 ());
  return true;
}

void
Ipv4TransportAddressTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Ipv4 Transport Address");
  os << "Address = " << m_address;
}

} // namespace ldp
} // namespace ns3
