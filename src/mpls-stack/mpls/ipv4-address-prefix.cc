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

#include <sstream>
#include <stdlib.h>

#include "ns3/log.h"
#include "ns3/assert.h"

#include "ipv4-address-prefix.h"

namespace ns3 {
namespace mpls {

Ipv4AddressPrefix::Ipv4AddressPrefix ()
  : m_address (Ipv4Address::GetAny ()),
    m_prefix (0)
{
}

Ipv4AddressPrefix::Ipv4AddressPrefix (const Ipv4Address &address)
{
  Set (address);
}

Ipv4AddressPrefix::Ipv4AddressPrefix (const Ipv4Address &address, uint8_t prefix)
{
  Set (address, prefix);
}

Ipv4AddressPrefix::Ipv4AddressPrefix (char const *address)
{
  Set (address);
}

Ipv4AddressPrefix::Ipv4AddressPrefix (const Ipv4AddressPrefix &fec)
{
  m_address = fec.m_address;
  m_prefix = fec.m_prefix;
}

Ipv4AddressPrefix::~Ipv4AddressPrefix ()
{
}

Ipv4AddressPrefix*
Ipv4AddressPrefix::Copy (void) const
{
  return new Ipv4AddressPrefix (*this);
}

void
Ipv4AddressPrefix::Set (const Ipv4Address &address)
{
  m_address = address;
  m_prefix = 32;
}

void
Ipv4AddressPrefix::Set (const Ipv4Address &address, uint8_t prefix)
{
  NS_ASSERT (prefix <= 32);
  m_address = address;
  m_prefix = prefix;
}

void
Ipv4AddressPrefix::Set (char const *address)
{
  NS_ASSERT (strlen (address) < 19);
  char tmp[19];
  char ch = 0;
  char *tp = tmp;
  uint8_t plen = 32;

  while ((ch = *address++) != '\0')
    {
      if (ch == '/')
        {
          plen = static_cast<uint32_t> (atoi (address));
          break;
        }
      else
        {
          *tp++ = ch;
        }
    }
  *tp = 0;

  Set (Ipv4Address (tmp), plen);
}

Ipv4Address
Ipv4AddressPrefix::GetAddress (void) const
{
  return m_address;
}

uint32_t
Ipv4AddressPrefix::GetMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  uint8_t match = 32 - m_prefix;

  if ((m_address.Get () >> match) == (header.GetDestination ().Get () >> match))
    {
      return FEC_MATCH_VALUE + match;
    }

  return -1;
}

void
Ipv4AddressPrefix::Print (std::ostream &os) const
{
  std::ostringstream oss;
  oss << m_address << "/" << (uint32_t)m_prefix;

  os << oss.str ();
}

std::ostream& operator<< (std::ostream& os, const Ipv4AddressPrefix &fec)
{
  fec.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
