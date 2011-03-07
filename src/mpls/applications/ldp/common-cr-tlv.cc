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

#include "common-cr-tlv.h"

namespace ns3 {
namespace ldp {

/**
 * LSPID TLV
 */

ENSURE_REGISTER_TLV (LspIdTLV);

const uint16_t LspIdTLV::TLV_TYPE = 0x0821;

Ptr<TypeLengthValue>
LspIdTLV::Factory (void)
{
  return Create<LspIdTLV> ();
}

LspIdTLV::LspIdTLV ()
  : m_setup (true),
    m_lspid (0),
    m_routerid (0)
{
}

LspIdTLV::LspIdTLV (bool setup, uint16_t lspid, uint32_t routerid)
  : m_setup (setup),
    m_lspid (lspid),
    m_routerid (routerid)
{
}

LspIdTLV::~LspIdTLV ()
{
}

void
LspIdTLV::SetSetup (bool setup)
{
  m_setup = setup;
}

bool
LspIdTLV::IsSetup (void) const
{
  return m_setup;
}

void
LspIdTLV::SetLspId (uint16_t lspid)
{
  m_lspid = lspid;
}

uint16_t
LspIdTLV::GetLspId (void) const
{
  return m_lspid;
}

void
LspIdTLV::SetRouterId (uint32_t routerid)
{
  m_routerid = routerid;
}

uint32_t
LspIdTLV::GetRouterId (void) const
{
  return m_routerid;
}

uint16_t
LspIdTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
LspIdTLV::GetLength (void) const
{
  return 8;
}

void
LspIdTLV::SerializeValue (Buffer::Iterator &start) const
{
  if (m_setup)
    {
      start.WriteU16 (0);
    }
  else
    {
      start.WriteHtonU16 (1);
    }

  start.WriteHtonU16 (m_lspid);
  start.WriteHtonU32 (m_routerid);
}

bool
LspIdTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 8)
    {
      return false;
    }

  uint16_t flg = start.ReadNtohU16 ();
  if (flg == 0)
    {
      m_setup = true;
    }
  else if (flg == 1)
    {
      m_setup = false;
    }
  else
    {
      return false;
    }

  m_lspid = start.ReadNtohU16 ();
  m_routerid = start.ReadNtohU32 ();
  return true;
}

void
LspIdTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "LSPID TLV");
  if (m_setup)
    {
      os << "setup ";
    }
  else
    {
      os << "modify ";
    }
  os << "lspid=" << m_lspid << " routerid=" << std::hex << m_routerid << std::dec;
}

ENSURE_REGISTER_TLV (ExplicitRouteTLV);

const uint16_t ExplicitRouteTLV::TLV_TYPE = 0x0800;

Ptr<TypeLengthValue>
ExplicitRouteTLV::Factory (void)
{
  return Create<ExplicitRouteTLV> ();
}

ExplicitRouteTLV::ExplicitRouteTLV ()
{
}

ExplicitRouteTLV::~ExplicitRouteTLV ()
{
  for (RouteHopsList::iterator i = m_routeHops.begin (); i != m_routeHops.end (); ++i)
    {
      *i = 0;
    }

  m_routeHops.clear ();
}

void
ExplicitRouteTLV::AddRouteHop (Ptr<const ExplicitRouteHopTLV> routeHop)
{
  m_routeHops.push_back (routeHop);
}

Ptr<const ExplicitRouteHopTLV>
ExplicitRouteTLV::GetFirstRouteHop (void) const
{
  if (m_routeHops.size () != 0)
    {
      return m_routeHops.front ();
    }

  return 0;
}

Ptr<const ExplicitRouteHopTLV>
ExplicitRouteTLV::GetSecondRouteHop (void) const
{
  if (m_routeHops.size () > 1)
    {
      return *(++m_routeHops.begin());
    }

  return 0;
}

void
ExplicitRouteTLV::RemoveFirstRouteHop (void)
{
  NS_ASSERT_MSG (m_routeHops.size () != 0, "ExplicitRouteTLV::RemoveFirstRouteHop (): no route hops");
  m_routeHops.pop_front ();
}

ExplicitRouteTLV::Iterator
ExplicitRouteTLV::Begin (void) const
{
  return m_routeHops.begin ();
}

ExplicitRouteTLV::Iterator
ExplicitRouteTLV::End (void) const
{
  return m_routeHops.end ();
}

Ptr<TypeLengthValue>
ExplicitRouteTLV::CopyValue (void) const
{
  return Copy ();
}

Ptr<ExplicitRouteTLV>
ExplicitRouteTLV::Copy (void) const
{
  Ptr<ExplicitRouteTLV> tlv = Create<ExplicitRouteTLV> ();
  for (RouteHopsList::const_iterator i = m_routeHops.begin (); i != m_routeHops.end (); ++i)
    {
      tlv->m_routeHops.push_back (DynamicCast<ExplicitRouteHopTLV> ((*i)->CopyValue ()));
    }
  return tlv;
}

uint16_t
ExplicitRouteTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
ExplicitRouteTLV::GetLength (void) const
{
  uint32_t length = 0;

  for (RouteHopsList::const_iterator i = m_routeHops.begin (); i != m_routeHops.end (); ++i)
    {
      length += 4 + (*i)->GetLength ();
    }

  return length;
}

void
ExplicitRouteTLV::SerializeValue (Buffer::Iterator &start) const
{
  NS_ASSERT_MSG (m_routeHops.size (), "ExplicitRouteTLV:Serialize (): no route hops to serialize");

  for (RouteHopsList::const_iterator i = m_routeHops.begin (); i != m_routeHops.end (); ++i)
    {
      start.WriteHtonU16 ((*i)->GetType ());
      start.WriteHtonU16 ((*i)->GetLength ());
      (*i)->SerializeValue (start);
    }
}

bool
ExplicitRouteTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  while (size >= 4)
    {
      uint16_t type = start.ReadNtohU16 ();
      uint16_t length = start.ReadNtohU16 ();
      size -= 4;

      if (length > size)
        {
          return false;
        }

      size -= length;

      Ptr<ExplicitRouteHopTLV> tlv = 0;
      switch (type)
      {
        case Ipv4ExplicitRouteHopTLV::TLV_TYPE:
          tlv = Create<Ipv4ExplicitRouteHopTLV> ();
          break;

//        case Ipv6ExplicitRouteHopTLV::TLV_TYPE:
//          tlv = Create<Ipv6ExplicitRouteHopTLV> ();
//          break;

//        case AsNumberExplicitRouteHopTLV::TLV_TYPE:
//          tlv = Create<AsNumberExplicitRouteHopTLV> ();
//          break;

//        case LspidExplicitRouteHopTLV::TLV_TYPE:
//          tlv = Create<LspidExplicitRouteHopTLV> ();
//          break;

        default:
          return false;
      }

      tlv->DeserializeValue (start, length);
      m_routeHops.push_back (tlv);
      start.Next (length);
    }

  return size == 0;
}

void
ExplicitRouteTLV::Print (std::ostream &os) const
{
}

/**
 * ER-Hop TLV
 */

ExplicitRouteHopTLV::ExplicitRouteHopTLV ()
  : m_loose (false)
{
}

ExplicitRouteHopTLV::~ExplicitRouteHopTLV ()
{
}

void
ExplicitRouteHopTLV::SetLoose (bool loose)
{
  m_loose = loose;
}

bool
ExplicitRouteHopTLV::IsLoose (void) const
{
  return m_loose;
}

/**
 * ER-Hop 1: The IPv4 prefix
 */

Ipv4ExplicitRouteHopTLV::Ipv4ExplicitRouteHopTLV ()
  : m_prefix (32),
    m_address (Ipv4Address::GetAny ())
{
}

Ipv4ExplicitRouteHopTLV::Ipv4ExplicitRouteHopTLV (const Ipv4Address &address, uint8_t prefix)
  : m_address (address)
{
  SetPrefix (prefix);
}

Ipv4ExplicitRouteHopTLV::~Ipv4ExplicitRouteHopTLV ()
{
}

void
Ipv4ExplicitRouteHopTLV::SetAddress (const Ipv4Address &address)
{
  m_address = address;
}

Ipv4Address
Ipv4ExplicitRouteHopTLV::GetAddress (void) const
{
  return m_address;
}

void
Ipv4ExplicitRouteHopTLV::SetPrefix (uint8_t prefix)
{
  NS_ASSERT_MSG (prefix <= 32, "Ipv4ExplicitRouteHopTLV:SetPrefix (): invalid prefix length");
  m_prefix = prefix;
}

uint8_t
Ipv4ExplicitRouteHopTLV::GetPrefix (void) const
{
  return m_prefix;
}

Ptr<TypeLengthValue>
Ipv4ExplicitRouteHopTLV::CopyValue (void) const
{
  return Copy ();
}

Ptr<Ipv4ExplicitRouteHopTLV>
Ipv4ExplicitRouteHopTLV::Copy (void) const
{
  return Create<Ipv4ExplicitRouteHopTLV> (m_address, m_prefix);
}

uint16_t
Ipv4ExplicitRouteHopTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
Ipv4ExplicitRouteHopTLV::GetLength (void) const
{
  return 8;
}

void
Ipv4ExplicitRouteHopTLV::SerializeValue (Buffer::Iterator &start) const
{
  if (m_loose)
    {
      start.WriteU8 (1 << 7);
    }
  else
    {
      start.WriteU8 (0);
    }

  start.WriteU16 (0);
  start.WriteU8 (m_prefix);
  start.WriteHtonU32 (m_address.Get());
}

bool
Ipv4ExplicitRouteHopTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  if (size != 8)
    {
      return false;
    }

  if ((start.ReadU8 () >> 7) == 1)
    {
      m_loose = true;
    }

  start.ReadU16 ();
  m_prefix = start.ReadU8 ();

  if (m_prefix > 32)
    {
      return false;
    }

  m_address.Set (start.ReadNtohU32 ());

  return true;
}

void
Ipv4ExplicitRouteHopTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Ipv4 Explicit Route Hop TLV");
  os << "address=" << m_address;
}

} // namespace ldp
} // namespace ns3
