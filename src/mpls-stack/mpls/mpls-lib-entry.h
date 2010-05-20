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

#ifndef MPLS_LIB_ENTRY_H
#define MPLS_LIB_ENTRY_H

#include <ostream>
#include <vector>

#include "ns3/simple-ref-count.h"

#include "forwarding-equivalence-class.h"

namespace ns3 {
namespace mpls {

class MplsLibEntry : public SimpleRefCount<MplsLibEntry>
{
public:
  enum LabelOperation
  {
    PUSH = 1,
    POP,
    SWAP
  };

  MplsLibEntry ();
  virtual ~MplsLibEntry ();

  void SetInLabel (uint32_t label);
  uint32_t GetInLabel (void) const;

  void SetInIfIndex (int32_t ifIndex);
  int32_t GetInIfIndex (void) const;

  void SetOutLabel (uint32_t label);
  uint32_t GetOutLabel (void) const;

  void SetOutIfIndex (int32_t ifIndex);
  int32_t GetOutIfIndex (void) const;

  void SetOperation (enum LabelOperation operation);
  enum LabelOperation GetOperation (void) const;

  const ForwardingEquivalenceClass* GetFec (void) const;
  void SetFec (const ForwardingEquivalenceClass &fec);

  void Print (std::ostream &os) const;

private:
  const ForwardingEquivalenceClass* m_fec;
  int32_t m_inIfIndex;
  uint32_t m_inLabel;
  int32_t m_outIfIndex;
  enum LabelOperation m_operation;
  uint32_t m_outLabel;
};

std::ostream& operator<< (std::ostream& os, const MplsLibEntry &entry);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LIB_ENTRY_H */
