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
#include "ns3/address-utils.h"
#include "ns3/ipv4-address.h"

#include "fec-tlv.h"

namespace ns3 {
namespace ldp {

ENSURE_REGISTER_TLV (FecTLV);

const uint16_t FecTLV::TLV_TYPE = 0x0100;

Ptr<TypeLengthValue>
FecTLV::Factory (void)
{
  return Create<FecTLV> ();
}

FecTLV::FecTLV ()
{
}

FecTLV::~FecTLV ()
{
  for (FecElementVector::iterator i = m_elements.begin(); i != m_elements.end(); ++i)
  {
    *i = 0;
  }

  m_elements.clear ();
}

void
FecTLV::AddElement (Ptr<const FecElement> element)
{
  m_elements.push_back (element);
}

Ptr<const FecElement>
FecTLV::GetElement (uint32_t i) const
{
  NS_ASSERT_MSG (i < m_elements.size (), "FecTLV:GetElement (): element index out of range");
  return m_elements[i];
}

uint32_t
FecTLV::GetNElements (void) const
{
  return m_elements.size ();
}

FecTLV::Iterator
FecTLV::Begin (void) const
{
  return m_elements.begin ();
}

FecTLV::Iterator
FecTLV::End (void) const
{
  return m_elements.end ();
}

uint16_t
FecTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
FecTLV::GetLength (void) const
{
  uint32_t length = 0; // message id size
  for (FecElementVector::const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
    {
      length += 1 + (*i)->GetLength (); // tlv fec element length
    }

  return length;
}

void
FecTLV::SerializeValue (Buffer::Iterator &start) const
{
  NS_ASSERT_MSG (m_elements.size (), "FecTLV:SerializeValue (): no elements to serialize");

  for (FecElementVector::const_iterator i = m_elements.begin (); i != m_elements.end (); ++i)
    {
      start.WriteU8 ((*i)->GetType ());
      (*i)->SerializeValue (start);
    }
}

bool
FecTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  while (size > 0)
  {
    uint8_t type = start.ReadU8 ();
    size--;

    Ptr<FecElement> element;
    bool result = true;
    switch (type)
    {
      case 0x1: // Wildcard
        element = Create<WildcardFecElement> ();
        break;

      case 0x2: // Prefix Address
        element = Create<PrefixFecElement> ();
        result = element->DeserializeValue (start);
        break;

      case 0x3: // Host Address
        element = Create<HostAddressFecElement> ();
        result = element->DeserializeValue (start);
        break;

      case 0x4: // CR-LSP
        element = Create<CrLspFecElement> ();
        break;

      default:
        return false;
    }

    if (!result)
      {
        return false;
      }

    m_elements.push_back (element);
    start.Next (element->GetLength ());
    size -= element->GetLength ();
  }

  return size == 0;
}

void
FecTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Fec TLV");
}

uint16_t
FecTLV::GetAddressFamily (const Address &address)
{
  if (Ipv4Address::IsMatchingType (address))
    {
      return 1; // IP Address Family
    }

  return 0; // unsupported
}

/* FEC Element */
FecElement::FecElement ()
{
}

FecElement::~FecElement ()
{
}

/* FEC Wildcard Element */
const uint8_t WildcardFecElement::FEC_ELEMENT_TYPE = 0x1;

WildcardFecElement::WildcardFecElement ()
{
}

WildcardFecElement::~WildcardFecElement ()
{
}

Ptr<FecTLV>
WildcardFecElement::CreateFecTLV (void)
{
  Ptr<FecTLV> tlv= Create<FecTLV> ();
  tlv->AddElement (Create<WildcardFecElement> ());
  return tlv;
}

uint8_t
WildcardFecElement::GetType (void) const
{
  return FEC_ELEMENT_TYPE;
}

uint32_t
WildcardFecElement::GetLength (void) const
{
  return 0;
}

void
WildcardFecElement::SerializeValue (Buffer::Iterator &start) const
{
}

bool
WildcardFecElement::DeserializeValue (Buffer::Iterator start)
{
  return true;
}

void
WildcardFecElement::Print (std::ostream &os) const
{
  os << "Wildcard";
}

/* FEC Address Prefix Element */
const uint8_t PrefixFecElement::FEC_ELEMENT_TYPE = 0x2;

PrefixFecElement::PrefixFecElement ()
  : m_prefix (0),
    m_address (Ipv4Address::GetAny ())
{
}

PrefixFecElement::PrefixFecElement (const Address &address, uint8_t prefix)
  : m_prefix (prefix),
    m_address (address)
{
}

PrefixFecElement::~PrefixFecElement ()
{
}

void
PrefixFecElement::SetAddress (const Address &address)
{
  m_address = address;
}

Address
PrefixFecElement::GetAddress (void) const
{
  return m_address;
}

void
PrefixFecElement::SetPrefix (uint8_t prefix)
{
  m_prefix = prefix;
}

uint8_t
PrefixFecElement::GetPrefix (void) const
{
  return m_prefix;
}

uint8_t
PrefixFecElement::GetType (void) const
{
  return FEC_ELEMENT_TYPE;
}

uint32_t
PrefixFecElement::GetLength (void) const
{
  return 3 + ((m_prefix + 7) >> 3);
}

void
PrefixFecElement::SerializeValue (Buffer::Iterator &start) const
{
  uint8_t buf[Address::MAX_SIZE];

  uint32_t addrLen = m_address.CopyTo (buf);
  uint32_t paddedLen = (m_prefix + 7) >> 3;

  NS_ASSERT_MSG (paddedLen <= addrLen, "PrefixFecElement::SerializeValue (): invalid prefix length");

  start.WriteHtonU16 (FecTLV::GetAddressFamily (m_address));
  start.WriteU8 (m_prefix);

  // XXX: RFC 3036: An address prefix encoded according to the Address Family field
  // buf in network byte order?
  start.Write (buf, paddedLen);
}

bool
PrefixFecElement::DeserializeValue (Buffer::Iterator start)
{
  uint16_t addrFamily = start.ReadNtohU16 ();
  m_prefix = start.ReadU8 ();
  uint32_t paddedLen = (m_prefix + 7) >> 3;

  switch (addrFamily)
  {
    case 1:
      if (paddedLen > 4)
        {
          return false;
        }
      uint8_t buf[4];
      start.Read (buf, paddedLen);
      m_address = Ipv4Address::Deserialize (buf);

    default:
      return false;
  }

  return true;
}

Ptr<FecTLV>
PrefixFecElement::CreateFecTLV (const Address &address, uint8_t prefix)
{
  Ptr<FecTLV> tlv= Create<FecTLV> ();
  tlv->AddElement (Create<PrefixFecElement> (address, prefix));
  return tlv;
}

void
PrefixFecElement::Print (std::ostream &os) const
{
  os << "Prefix " << m_address << "/" << (uint32_t)m_prefix;
}

/* FEC Host Address Element */
const uint8_t HostAddressFecElement::FEC_ELEMENT_TYPE = 0x3;

HostAddressFecElement::HostAddressFecElement ()
  : m_address (Ipv4Address::GetAny ())
{
}

HostAddressFecElement::HostAddressFecElement (const Address &address)
  : m_address (address)
{
}

HostAddressFecElement::~HostAddressFecElement ()
{
}

void
HostAddressFecElement::SetAddress (const Address &address)
{
  m_address = address;
}

Address
HostAddressFecElement::GetAddress (void) const
{
  return m_address;
}

uint8_t
HostAddressFecElement::GetType (void) const
{
  return FEC_ELEMENT_TYPE;
}

uint32_t
HostAddressFecElement::GetLength (void) const
{
  return 3 + m_address.GetLength ();
}

void
HostAddressFecElement::SerializeValue (Buffer::Iterator &start) const
{
  uint16_t addrFamily = FecTLV::GetAddressFamily (m_address);

  start.WriteHtonU16 (addrFamily);
  start.WriteU8 (m_address.GetLength ());

  switch (addrFamily)
  {
    case 1:
      start.WriteHtonU32 (Ipv4Address::ConvertFrom (m_address).Get ());
      break;

    default:
      NS_ASSERT_MSG (false, "HostAddressFecElement:SerializeValue (): unsupported address family");
  }
}

bool
HostAddressFecElement::DeserializeValue (Buffer::Iterator start)
{
  uint16_t addrFamily = start.ReadNtohU16 ();
  uint8_t length = start.ReadU8 ();

  switch (addrFamily)
  {
    case 1:
      if (length != 4)
        {
          return false;
        }

      m_address = Ipv4Address (start.ReadNtohU32 ());
      break;

    default:
      return false;
  }

  return true;
}

Ptr<FecTLV>
HostAddressFecElement::CreateFecTLV (const Address &address)
{
  Ptr<FecTLV> tlv= Create<FecTLV> ();
  tlv->AddElement (Create<HostAddressFecElement> (address));
  return tlv;
}

void
HostAddressFecElement::Print (std::ostream &os) const
{
  os << "Host " << m_address;
}

/* FEC CR-LSP Element */
const uint8_t CrLspFecElement::FEC_ELEMENT_TYPE = 0x4;

CrLspFecElement::CrLspFecElement ()
{
}

CrLspFecElement::~CrLspFecElement ()
{
}

uint8_t
CrLspFecElement::GetType (void) const
{
  return FEC_ELEMENT_TYPE;
}

uint32_t
CrLspFecElement::GetLength (void) const
{
  return 0;
}

void
CrLspFecElement::SerializeValue (Buffer::Iterator &start) const
{
}

bool
CrLspFecElement::DeserializeValue (Buffer::Iterator start)
{
  return true;
}

Ptr<FecTLV>
CrLspFecElement::CreateFecTLV (void)
{
  Ptr<FecTLV> tlv = Create<FecTLV> ();
  tlv->AddElement (Create<CrLspFecElement> ());
  return tlv;
}

void
CrLspFecElement::Print (std::ostream &os) const
{
  os << "CR-LSP";
}

} // namespace ldp
} // namespace ns3
