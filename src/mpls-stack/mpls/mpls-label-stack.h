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

#ifndef MPLS_LABEL_STACK_H
#define MPLS_LABEL_STACK_H

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <deque>

#include "ns3/header.h"
#include "mpls-generic.h"

namespace ns3 {
namespace mpls {

typedef uint32_t Shim;

/**
 * \ingroup mpls
 * \brief
 * Label stack entry
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Label
 *  |                Label                  | Exp |S|       TTL     | Stack
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Entry
 *
 *
 * The label stack is represented as a sequence of "label stack entries".
 * For more infomation see RFC 3032 (http://www.ietf.org/rfc/rfc3032.txt)
 */
class MplsLabelStack : public Header
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  /**
   * \brief Create an empty stack.
   */
  MplsLabelStack ();
  /**
   * \brief Destructor
   */
  virtual ~MplsLabelStack ();
  /**
   * \brief Removes and returns the stack's top entry
   */
  void Pop (void);
  /**
   * \brief Add a new entry to the top of the stack
   */
  void Push (Shim entry);
  /**
   * \brief Retrieves the stack's top entry
   */
  Shim Peek (void) const;
  /**
   * \brief Detects whether the stack is empty
   */
  bool IsEmpty (void) const;
  /**
   * \brief Removes all entries from the stack
   */
  bool Clear (void);

  // Functions defined in base class Header
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  typedef std::deque<Shim> Stack;
  Stack m_entries;
};


namespace shimUtils {
  /**
   * \brief Calculates and returns shim
   */
  Shim GetShim (Label label);
  Shim GetShim (Label label, uint8_t ttl);
  Shim GetShim (Label label, uint8_t ttl, uint8_t exp);
  /**
   * \brief Set label value
   */ 
  void SetLabel (Shim& shim, Label label);
  /**
   * \brief Returns label value
   */ 
  Label GetLabel (Shim shim);  
  /**
   * \brief Set value of 'Experimental Use' field
   */
  void SetExperimentalUse (Shim& shim, uint8_t exp);
  /**
   * \Brief Returns value of 'Experimental Use' field
   */
  uint8_t GetExperimentalUse (Shim shim);
  /**
   * \brief Set value of 'Time To Live' field
   */
  void SetTimeToLive (Shim& shim, uint8_t ttl);
 /**
   * \brief Returns value of 'Time To Live' field
   */
  uint8_t GetTimeToLive (Shim shim);
  /**
   * \brief Returns true if 'Bottom of Stack' flag is set
   */
  bool IsBottomOfStack (Shim shim);
  /**
   * \brief Returns string representation of the stack entry
   */
  std::string AsString (Shim shim);
} // namespace stackUtils


} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_STACK_H */
