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

#include "ipv4-host-address.h"

namespace ns3 {
namespace mpls {

Ipv4HostAddress::Ipv4HostAddress ()
  : m_address (Ipv4Address::GetAny ())
{
}

Ipv4HostAddress::Ipv4HostAddress (const Ipv4Address &address)
  : m_address (address)
{
}

Ipv4HostAddress::Ipv4HostAddress (const Ipv4HostAddress &fec)
{
  m_address = fec.m_address;
}

Ipv4HostAddress::~Ipv4HostAddress ()
{
}

Ipv4HostAddress*
Ipv4HostAddress::Copy (void) const
{
  return new Ipv4HostAddress (*this);
}

void
Ipv4HostAddress::Set (const Ipv4Address &address)
{
  m_address = address;
}

Ipv4Address
Ipv4HostAddress::GetAddress (void) const
{
  return m_address;
}

uint32_t
Ipv4HostAddress::GetMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  if (m_address.IsEqual (header.GetDestination ()))
    {
      return FEC_MATCH_VALUE;
    }

  return -1;
}

void
Ipv4HostAddress::Print (std::ostream &os) const
{
  std::ostringstream oss;
  oss << m_address;
  os << oss.str ();
}

std::ostream& operator<< (std::ostream& os, const Ipv4HostAddress &fec)
{
  fec.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
