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

#include "mpls-generic.h"

namespace ns3 {
namespace mpls {

////////////////////////////////////////////////////////////////
// MplsLabel class
////////////////////////////////////////////////////////////////

const int32_t MplsLabel::IPV4_EXPLICIT_NULL = 0;
const int32_t MplsLabel::ROUTE_ALERT = 1;
const int32_t MplsLabel::IPV6_EXPLICIT_NULL = 2;
const int32_t MplsLabel::IMPLICIT_NULL = 3;

MplsLabel::MplsLabel ()
  : m_label (-1)
{
}

MplsLabel::MplsLabel (int32_t label)
{
  m_label = label;
}

inline
bool
MplsLabel::IsInvalid (void) const
{
  return m_label & 0xfff0000c;
}

inline
bool
MplsLabel::IsIpv4ExplicitNull (void) const
{
  return m_label == IPV4_EXPLICIT_NULL;
}

inline
bool
MplsLabel::IsRouteAlert (void) const
{
  return m_label == ROUTE_ALERT;
}

inline
bool
MplsLabel::IsIpv6ExplicitNull (void) const
{
  return m_label == IPV6_EXPLICIT_NULL;
}

inline
bool
MplsLabel::IsImplicitNull (void) const
{
  return m_label == IMPLICIT_NULL;
}

inline
int32
MplsLabel::GetValue (void) const
{
  return m_label;
}

inline
MplsLabel
MplsLabel::GetIpv4ExplicitNull (void)
{
  return MplsLabel (IPV4_EXPLICIT_NULL);
}

inline
MplsLabel
MplsLabel::GetRouteAlert (void)
{
  return MplsLabel (ROUTE_ALERT);
}

inline
MplsLabel
MplsLabel::GetIpv6ExplicitNull (void)
{
  return MplsLabel (IPV6_EXPLICIT_NULL);
}

inline
bool
MplsLabel::operator== (const MplsLabel &label)
{
  return m_label == label.m_label;
}

inline
bool
MplsLabel::operator!= (const MplsLabel &label)
{
  return m_label != label.m_label;
}

inline
MplsLabel
MplsLabel::GetImplicitNull (void)
{
  return MplsLabel (IMPLICIT_NULL);
}

void
MplsLabel::Print (std::ostream &os) const
{
  if (IsInvalid ())
    {
      os << "Invalid";
      return;
    }

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
    os << m_label;
  }
}

std::ostream& operator<< (std::ostream& os, const MplsLabel &label)
{
  label.Print (os);
  return os;
}

////////////////////////////////////////////////////////////////
// MplsOperation class
////////////////////////////////////////////////////////////////

MplsOperation::MplsOperation ()
{
}

MplsOperation::~MplsOperation ()
{
}

} // namespace mpls
} // namespace ns3
