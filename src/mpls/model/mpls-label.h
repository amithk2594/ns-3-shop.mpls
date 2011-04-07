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

#ifndef MPLS_LABEL_H
#define MPLS_LABEL_H

#include <ostream>
#include <stdint.h>

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief A representation of a MPLS label.
 *
 * This class provides a MPLS label defined in Multiprotocol Label Switching Architecture [RFC3031],
 * MPLS Label Stack Encoding [RFC3032], Generalized Multiprotocol Label Switching (GMPLS) Architecture [RFC3471].
 */
class Label
{
public:
  /**
   * @brief IPV4 explicit null label value
   */
  static const uint32_t IPV4_EXPLICIT_NULL;
  /**
   * @brief Route alert label value
   */
  static const uint32_t ROUTE_ALERT;
  /**
   * @brief IPV6 explicit null label value
   */
  static const uint32_t IPV6_EXPLICIT_NULL;
  /**
   * @brief implicit null label value
   */
  static const uint32_t IMPLICIT_NULL;
  /**
   * @brief construct label
   * @param label label value
   */
  Label (uint32_t value);
  /**
   * @brief Get label value
   */
  operator uint32_t (void) const;
  /**
   * @brief Get IPv4 explicit null label
   */
  static Label GetIpv4ExplicitNull (void);
  /**
   * @brief Get IPv6 explicit null label
   */
  static Label GetIpv6ExplicitNull (void);
  /**
   * @brief Get route alert label
   * @return Route alert label
   */
  static Label GetRouteAlert (void);
  /**
   * @brief Get null label
   * @return Implicit null label
   */
  static Label GetImplicitNull (void);

private:
  uint32_t m_value;

  friend std::ostream& operator<< (std::ostream& os, const Label &label);
};

/**
 * \brief output operation for Label
 */
std::ostream& operator<< (std::ostream& os, const Label& label);


namespace shim {

/**
 * @brief Calculates and returns the shim
 */
inline uint32_t Get (Label label)
{
  return uint32_t(label) << 12;
}

///**
// * @brief Calculates and returns the shim
// */  
//inline uint32_t Get (Label label, uint8_t exp)
//{
//  return (uint32_t(label) << 12) | (exp << 9);
//}

/**
 * @brief Returns the label value
 */ 
inline uint32_t GetLabel (uint32_t s)
{
  return s >> 12;
}

/**
 * @Brief Returns the value of 'Experimental Use' field
 */
inline uint8_t GetExp (uint32_t s)
{
  return (s >> 9) & 0x7;
}

/**
 * @brief Returns the shim with new 'Time To Live' field value
 */
inline uint8_t GetTtl (uint32_t s)
{
  return s;
}

/**
 * @brief Returns the value of 'Time To Live' field
 */
inline bool IsBos (uint32_t s)
{
  return (s >> 8) & 1;
}

/**
 * @brief Set value of 'Time To Live' field
 */
inline void SetTtl (uint32_t& s, uint8_t ttl)
{
  s = (s & 0xffffff00) | ttl;
}

/**
 * @brief Set value of 'Time To Live' field
 */
inline uint32_t SetTtl2 (uint32_t s, uint8_t ttl)
{
  return (s & 0xffffff00) | ttl;
}


/**
 * @brief Returns the new shim with bos set
 */
inline uint32_t SetBos (uint32_t s)
{
  return s | 0x100;
}

/**
 * @brief Returns the new shim with bos clear
 */
inline uint32_t ClearBos (uint32_t s)
{
  return s & 0xfffffeff;
}

} // namespace shim
} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_H */
