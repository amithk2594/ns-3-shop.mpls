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

#include <ostream>
#include <deque>
#include <stdint.h>

#include "ns3/header.h"
#include "mpls-generic.h"

namespace ns3 {
namespace mpls {

class MplsLabelStack;

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
 * For more infomation see RFC 3032 (http://www.ietf.org/rfc/rfc3032.txt)
 */
class MplsStackEntry
{
public:
  /**
   * \brief Create an empty entry
   */
  MplsStackEntry ();
  /**
   * \brief Create an entry with label
   */
  MplsStackEntry (const MplsLabel &label);
  /**
   * \brief Destructor
   */
  virtual ~MplsStackEntry ();
  /**
   * \brief Set the actual value of the Label
   * \param label
   */
  void SetLabel (const MplsLabel &label);
  /**
   * \returns label
   */
  const MplsLabel& GetLabel (void) const;
  /**
   * \brief Set three-bit exp field value
   * \param exp
   */
  void SetExp (uint8_t exp);
  /**
   * \returns Value of the exp field
   */
  uint8_t GetExp (void) const;
  /**
   * \brief Set time-to-live value
   * \param ttl eight-bit time-to-live value
   */
  void SetTtl (uint8_t ttl);
  /**
   * \returns time-to-live field value
   */
  uint8_t GetTtl (void) const;
  /**
   * \returns True if this entry is the last entry in the label stack
   */
  bool IsBos (void) const;
  /**
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  // this functions is used by MplsLabelStack
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

  MplsLabel m_label;
  uint8_t   m_exp;
  uint8_t   m_ttl;
  bool      m_bos;

  friend class MplsLabelStack;
};

std::ostream& operator<< (std::ostream& os, const MplsStackEntry &entry);

/**
 * \ingroup mpls
 * \brief
 * The label stack is represented as a sequence of "label stack entries".
 * Each label stack entry is represented by 4 octets.
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
   * \brief pop last entry
   */
  void Pop (void);
  /**
   * \brief push entry to the stack
   * \param entry
   */
  void Push (const MplsStackEntry &entry);
  /**
   * \returns top entry of the stack
   */
  MplsStackEntry& GetTop (void);
  /**
   * \returns top entry of the stack
   */
  const MplsStackEntry& GetTop (void) const;
  /**
   * \returns true if stack is empty
   */
  bool IsEmpty (void) const;

  // Functions defined in base class Header
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  typedef std::deque<MplsStackEntry> MplsStackEntryVector;
  MplsStackEntryVector m_entries;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_STACK_H */
