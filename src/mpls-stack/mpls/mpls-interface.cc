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

//#include "ns3/assert.h"

#include "mpls-fib.h"

namespace ns3 {
namespace mpls {

const int32_t MplsLabel::IPV4_EXPLICIT_NULL = 0;
const int32_t MplsLabel::ROUTE_ALERT = 1;
const int32_t MplsLabel::IPV6_EXPLICIT_NULL = 2;
const int32_t MplsLabel::IMPLICIT_NULL = 3;
const int32_t MplsLabel::RESERVED_LABEL = 4;
const int32_t MplsLabel::MIN_LABEL = 0x10;
const int32_t MplsLabel::MAX_LABEL = 0xfffff;

MplsLabel::MplsLabel ()
  : m_label (-1)
{
}

MplsLabel::MplsLabel (int32_t label)
{
  m_label = label;
}

bool
MplsLabel::IsValid (void) const
{
  if (m_label < 0 || m_label > MAX_LABEL)
    {
      return false;
    }

  // reserved labels
  if (m_label >= RESERVED_LABEL && m_label < MIN_LABEL)
    {
      return false;
    }

  return true;
}

bool
MplsLabel::IsIpv4ExplicitNull (void) const
{
  return m_label == IPV4_EXPLICIT_NULL;
}

bool
MplsLabel::IsRouteAlert (void) const
{
  return m_label == ROUTE_ALERT;
}

bool
MplsLabel::IsIpv6ExplicitNull (void) const
{
  return m_label == IPV6_EXPLICIT_NULL;
}

bool
MplsLabel::IsImplicitNull (void) const
{
  return m_label == IMPLICIT_NULL;
}

MplsLabel
MplsLabel::GetIpv4ExplicitNull (void)
{
  return MplsLabel (IPV4_EXPLICIT_NULL);
}

MplsLabel
MplsLabel::GetRouteAlert (void)
{
  return MplsLabel (ROUTE_ALERT);
}

MplsLabel
MplsLabel::GetIpv6ExplicitNull (void)
{
  return MplsLabel (IPV6_EXPLICIT_NULL);
}

bool
MplsLabel::operator== (const MplsLabel &label)
{
  return m_label == label.m_label;
}

bool
MplsLabel::operator!= (const MplsLabel &label)
{
  return m_label != label.m_label;
}

MplsLabel
MplsLabel::GetImplicitNull (void)
{
  return MplsLabel (IMPLICIT_NULL);
}

void
MplsLabel::Print (std::ostream &os) const
{
  switch (m_label)
  {
  case IPV4_EXPLICIT_NULL:
    os << "Ipv4ExplicitNull";
    break;

  case IPV6_EXPLICIT_NULL:
    os << "Ipv6ExplicitNull";
    break;

  case ROUTE_ALERT:
    os << "RouteAlert";
    break;

  case IMPLICIT_NULL:
    os << "ImplicitNull";
    break;

  default:
    if (IsValid ())
      {
        os << m_label;
      }
    else
      {
        os << "Invalid";
      }
  }
}

std::ostream& operator<< (std::ostream& os, const MplsLabel &label)
{
  label.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
