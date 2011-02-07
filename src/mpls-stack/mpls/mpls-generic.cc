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
namespace labelUtils {

inline
bool
IsValidLabel (Label label)
{
  return !(label & 0xfff00000);
}

inline
bool
IsIpv4ExplicitNullLabel (Label label)
{
  return label == IPV4_EXPLICIT_NULL_LABEL;
}

inline
bool
IsRouteAlertLabel (Label label)
{
  return label == ROUTE_ALERT_LABEL;
}

inline
bool
IsIpv6ExplicitNullLabel (Label label)
{
  return label == IPV6_EXPLICIT_NULL_LABEL;
}

inline
bool
IsImplicitNullLabel (Label label)
{
  return label == IMPLICIT_NULL_LABEL;
}

std::string
AsString (Label label)
{
  std::ostringstream os (std::ostringstream::out);

  if (IsValidLabel (label))
    {
      switch (label)
      {
        case IPV4_EXPLICIT_NULL_LABEL:
          os << "Ipv4 Explicit NULL label";
          break;

        case IPV6_EXPLICIT_NULL_LABEL:
          os << "Ipv6 Explicit NULL label";
          break;

        case ROUTE_ALERT_LABEL:
          os << "Route Alert label";
          break;

        case IMPLICIT_NULL_LABEL:
          os << "Implicit NULL label";
          break;

        default:
          if (label < 16) 
            {
              os << "Reserved label " << label;
            }
          else
            {
              os << label;
            }
      }
    }
  else
    {
      os << "Invalid label";
    }

  return os.str();
}

} // namespace labelUtils

} // namespace mpls
} // namespace ns3
