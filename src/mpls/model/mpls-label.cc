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
  NS_ASSERT_MSG (value > 0xf && value <= 0xfffff, "Invalid label value");
}

Label::Label (uint32_t value, uint32_t t)
  : m_value (value)
{
}

Label::operator uint32_t (void) const
{
  return m_value;
}

const Label&
Label::GetIpv4ExplicitNull (void)
{
  static Label label (IPV4_EXPLICIT_NULL, 0);
  return label;
}

const Label&
Label::GetRouteAlert (void)
{
  static Label label (ROUTE_ALERT, 0);
  return label;
}

const Label&
Label::GetIpv6ExplicitNull (void)
{
  static Label label (IPV6_EXPLICIT_NULL, 0);
  return label;
}

const Label&
Label::GetImplicitNull (void)
{
  static Label label (IMPLICIT_NULL, 0);
  return label;
}

std::ostream& operator<< (std::ostream& os, const Label &label)
{
  switch (label)
  {
    case Label::IPV4_EXPLICIT_NULL:
      os << "Ipv4 Explicit NULL";
      break;

    case Label::IPV6_EXPLICIT_NULL:
      os << "Ipv6 Explicit NULL";
      break;

    case Label::ROUTE_ALERT:
      os << "Route Alert";
      break;

    case Label::IMPLICIT_NULL:
      os << "Implicit NULL";
      break;

    default:
      os << "Label " << label.m_value;
  }

  return os;
}

} // namespace mpls
} // namespace ns3
