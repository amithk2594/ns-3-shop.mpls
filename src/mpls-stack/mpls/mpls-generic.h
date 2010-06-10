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
#include <ostream>

#include "mpls-label-stack.h"

namespace ns3 {
namespace mpls {

class MplsLabelStack;

/**
 * \ingroup mpls
 * \brief The Label Switching Router (LSR) identifier.
 */
typedef uint32_t MplsLsrIdentifier;

/**
 * \ingroup mpls
 * \brief A representation of a MPLS label.
 *
 * This class provides a MPLS label defined in Multiprotocol Label Switching Architecture [RFC3031],
 * MPLS Label Stack Encoding [RFC3032], Generalized Multiprotocol Label Switching (GMPLS) Architecture [RFC3471].
 */
class MplsLabel
{
public:
  /**
   * \brief IPV4 explicit null label value
   */
  static const int32_t IPV4_EXPLICIT_NULL;
  /**
   * \brief Route alert label value
   */
  static const int32_t ROUTE_ALERT;
  /**
   * \brief IPV6 explicit null label value
   */
  static const int32_t IPV6_EXPLICIT_NULL;
  /**
   * \brief implicit null label value
   */
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
   * \brief Check if label is invalid
   * \returns true if label is invalid
   */
  bool IsInvalid (void) const;
  /**
   * \brief Check if label is ipv4 explicit null label
   * \returns true if is ipv4 explicit null label
   */
  bool IsIpv4ExplicitNull (void) const;
  /**
   * \brief Check if label is ipv6 explicit null label
   * \returns true if is ipv6 explicit null label
   */
  bool IsIpv6ExplicitNull (void) const;
  /**
   * \brief Check if label is route alert label
   * \returns true if is route alert label
   */
  bool IsRouteAlert (void) const;
  /**
   * \brief Check if label is implicit null label
   * \return true if is implicit null label
   */
  bool IsImplicitNull (void) const;
  /**
   * \brief Get label value
   * \returns label value
   */
  int32 GetValue (void) const;
  /**
   * \brief Get IPv4 explicit null label
   * \returns Ipv4 explicit null label
   */
  static MplsLabel GetIpv4ExplicitNull (void);
  /**
   * \brief Get IPv6 explicit null label
   * \returns Ipv6 explicit null label
   */
  static MplsLabel GetIpv6ExplicitNull (void);
  /**
   * \brief Get route alert label
   * \returns Route alert label
   */
  static MplsLabel GetRouteAlert (void);
  /**
   * \brief Get null label
   * \returns Implicit null label
   */
  static MplsLabel GetImplicitNull (void);
  /**
   * \brief Print label value
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

public:
  /**
   * \brief 'equal-to' operator
   */
  bool operator== (const MplsLabel &label);
  /**
   * \brief 'not-equal-to' operator
   */
  bool operator!= (const MplsLabel &label);

private:
  int32_t m_label;
};

/**
 * \brief output operation for MplsLabel
 */
std::ostream& operator<< (std::ostream& os, const MplsLabel &label);

/**
 * \ingroup mpls
 * \brief A MPLS label forwarding operation abstract base class.
 *
 * This is an abstract base class for a MPLS label forwarding operation defined in
 * Multiprotocol Label Switching Architecture [RFC3031].
 */
class MplsOperation
{
public:
  /**
   * \brief Destructor
   */
  virtual ~MplsOperation ();
  /**
   * \brief Execute operation on the label stack
   * \param stack Mpls label stack to perform operation
   * \returns false if operation executing failed
   */
  virtual bool Execute (MplsLabelStack &stack) const = 0;
  /**
   * \brief Print operation
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  MplsOperation ();
};

/**
 * \brief output operation for MplsOperation
 */
std::ostream& operator<< (std::ostream& os, const MplsOperation &op);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_H */
