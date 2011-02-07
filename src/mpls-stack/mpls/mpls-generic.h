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

/**
 * \defgroup mpls
 */
#ifndef MPLS_GENERIC_H
#define MPLS_GENERIC_H

#include <stdint.h>
#include <iostream>
#include <sstream>

//#include "mpls-label-stack.h"

namespace ns3 {
namespace mpls {

//class MplsLabelStack;

/**
 * \ingroup mpls
 * \brief The Label Switching Router (LSR) identifier.
 */
typedef uint32_t LsrIdentifier;

/**
 * \ingroup mpls
 * \brief A representation of a MPLS label.
 *
 * This type provides a MPLS label defined in Multiprotocol Label Switching Architecture [RFC3031],
 * MPLS Label Stack Encoding [RFC3032], Generalized Multiprotocol Label Switching (GMPLS) Architecture [RFC3471].
 */
typedef int32_t Label;


namespace labelUtils {
  /**
   * \brief IPV4 explicit null label value
   */
  static const Label IPV4_EXPLICIT_NULL_LABEL = 0;
  /**
   * \brief Route alert label value
   */
  static const Label ROUTE_ALERT_LABEL = 1;
  /**
   * \brief IPV6 explicit null label value
   */
  static const Label IPV6_EXPLICIT_NULL_LABEL = 2;
  /**
   * \brief implicit null label value
   */
  static const Label IMPLICIT_NULL_LABEL = 3;
  /**
   * \brief Check if label has valid value
   */
  bool IsValidLabel (Label label);
  /**
   * \brief Check if label is ipv4 explicit null label
   * \param label label value   
   */
  bool IsIpv4ExplicitNullLabel (Label label);
  /**
   * \brief Check if label is ipv6 explicit null label
   * \param label label value   
   */
  bool IsIpv6ExplicitNullLabel (Label label);
  /**
   * \brief Check if label is route alert label
   * \param label label value   
   */
  bool IsRouteAlertLabel (Label label);
  /**
   * \brief Check if label is implicit null label
   * \param label label value   
   */
  bool IsImplicitNullLabel (Label label);
  /**
   * \brief Print label value
   * \param os the stream to print to
   * \param label label value
   */
  std::string AsString (Label label);
} // namespace labelUtils

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_H */
