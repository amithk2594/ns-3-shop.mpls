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

#ifndef MPLS_LABEL_H
#define MPLS_LABEL_H

#include <stdint.h>
#include <ostream>

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief Mpls label. For mor information see RFC 3032 (http://www.ietf.org/rfc/rfc3032.txt)
 */
class MplsLabel
{
public:
  static const int32_t IPV4_EXPLICIT_NULL;
  static const int32_t ROUTE_ALERT;
  static const int32_t IPV6_EXPLICIT_NULL;
  static const int32_t IMPLICIT_NULL;

  /**
   * \brief construct invalid label
   */
  MplsLabel ();
  /**
   * \brief construct label
   * \param label label value
   */
  MplsLabel (int32_t label);
  /**
   * \returns true if label is invalid
   */
  bool IsInvalid (void) const;
  /**
   * \returns true if is ipv4 explicit null label
   */
  bool IsIpv4ExplicitNull (void) const;
  /**
   * \returns true if is ipv6 explicit null label
   */
  bool IsIpv6ExplicitNull (void) const;
  /**
   * \returns true if is route alert label
   */
  bool IsRouteAlert (void) const;
  /**
   * \return true if is implicit null label
   */
  bool IsImplicitNull (void) const;
  /**
   * \returns label value
   */
  int32 GetValue (void) const;
  /**
   * \returns Ipv4 explicit null label
   */
  static MplsLabel GetIpv4ExplicitNull (void);
  /**
   * \returns Ipv6 explicit null label
   */
  static MplsLabel GetIpv6ExplicitNull (void);
  /**
   * \returns Route alert label
   */
  static MplsLabel GetRouteAlert (void);
  /**
   * \returns Implicit null label
   */
  static MplsLabel GetImplicitNull (void);
  /**
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

public:
  bool operator== (const MplsLabel &label);
  bool operator!= (const MplsLabel &label);

private:
  int32_t m_label;
};

std::ostream& operator<< (std::ostream& os, const MplsLabel &label);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_H */
