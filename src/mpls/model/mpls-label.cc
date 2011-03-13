/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#include "ns3/assert.h"

#include "mpls-label.h"

namespace ns3 {
namespace mpls {

const uint32_t Label::IPV4_EXPLICIT_NULL = 0;
const uint32_t Label::ROUTE_ALERT = 1;
const uint32_t Label::IPV6_EXPLICIT_NULL = 2;
const uint32_t Label::IMPLICIT_NULL = 3;

Label::Label (uint32_t value)
  : m_value (value)
{
  NS_ASSERT_MSG (value <= 0xfffff, "Invalid label value");
}

Label::operator uint32_t (void) const
{
  return m_value;
}

Label
Label::GetIpv4ExplicitNull (void)
{
  return Label (IPV4_EXPLICIT_NULL);
}

Label
Label::GetRouteAlert (void)
{
  return Label (IPV4_EXPLICIT_NULL);
}

Label
Label::GetIpv6ExplicitNull (void)
{
  return Label (IPV4_EXPLICIT_NULL);
}

Label
Label::GetImplicitNull (void)
{
  return Label (IPV4_EXPLICIT_NULL);
}

std::ostream& operator<< (std::ostream& os, const Label &label)
{
  switch (label)
  {
    case Label::IPV4_EXPLICIT_NULL:
      os << "ipv4_explicit_null";
      break;

    case Label::IPV6_EXPLICIT_NULL:
      os << "ipv6_explicit_null";
      break;

    case Label::ROUTE_ALERT:
      os << "route_alert";
      break;

    case Label::IMPLICIT_NULL:
      os << "implicit_nULL";
      break;

    default:
      os << label.m_value;
  }

  return os;
}

} // namespace mpls
} // namespace ns3
