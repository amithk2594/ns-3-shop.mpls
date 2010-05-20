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
#include <vector>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/header.h"

namespace ns3 {
namespace mpls {

class MplsLabelStack;

//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Label
//  |                Label                  | Exp |S|       TTL     | Stack
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Entry
class MplsLabelStackEntry : public SimpleRefCount<MplsLabelStackEntry>
{
public:
  static const uint32_t MPLS_LABEL_IPV4NULL = 0;
  static const uint32_t MPLS_LABEL_ROUTERALERT = 1;
  static const uint32_t MPLS_LABEL_IPV6NULL = 2;
  static const uint32_t MPLS_LABEL_IMPLNULL = 3;
  static const uint32_t MPLS_LABEL_RESERVED_MAX = 15;
  static const uint32_t MPLS_LABEL_MAX = (1 << 20) - 1;

  MplsLabelStackEntry ();
  virtual ~MplsLabelStackEntry ();

  void SetLabel (uint32_t label);
  uint32_t GetLabel (void) const;
  void SetExp (uint8_t exp);
  uint8_t GetExp (void) const;
  bool IsBos (void) const;
  void SetTtl (uint8_t ttl);
  uint8_t GetTtl (void) const;

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

private:
  friend class MplsLabelStack;

  uint32_t m_label;
  uint8_t m_exp;
  bool m_bos;
  uint8_t m_ttl;
};

std::ostream& operator<< (std::ostream& os, const MplsLabelStackEntry &entry);

class MplsLabelStack : public Header
{
public:
  MplsLabelStack ();
  virtual ~MplsLabelStack ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  Ptr<MplsLabelStackEntry> Pop (void);
  Ptr<MplsLabelStackEntry> Push (uint32_t label);
  Ptr<MplsLabelStackEntry> Swap (uint32_t label);

  typedef std::vector<Ptr<MplsLabelStackEntry> >::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;
  Ptr<MplsLabelStackEntry> GetEntry (uint32_t i) const;
  Ptr<MplsLabelStackEntry> GetTopEntry (void) const;
  uint32_t GetNEntries (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  typedef std::vector<Ptr<MplsLabelStackEntry> > MplsLabelStackEntryVector;
  MplsLabelStackEntryVector m_entries;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_STACK_H */
