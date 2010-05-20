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

#ifndef LABEL_INFORMATION_BASE_HELPER_H
#define LABEL_INFORMATION_BASE_HELPER_H

#include "ns3/ptr.h"
#include "ns3/node.h"

#include "mpls-routing-protocol.h"
#include "mpls-lib-entry.h"
#include "forwarding-equivalence-class.h"

namespace ns3 {

using namespace mpls;

class LabelInformationBaseHelper
{
public:
  LabelInformationBaseHelper ();
  LabelInformationBaseHelper (Ptr<Node> node);
  virtual ~LabelInformationBaseHelper ();

  void SetNode (Ptr<Node> node);

  /* incomming IP, outgoing interface, outgoing label */
  void InstallEntry (const ForwardingEquivalenceClass &fec,
                     int32_t outIfIndex,
                     uint32_t outLabel) const;

  /* just shortcut, outIfIndex = -1 */
  void InstallEntry (const ForwardingEquivalenceClass &fec,
                     uint32_t outLabel) const;

  /* incoming interface, incomming label, outgoing interface, outgoing label */
  void InstallEntry (int32_t inIfIndex,
                     uint32_t inLabel,
                     int32_t outIfIndex,
                     enum MplsLibEntry::LabelOperation op,
                     uint32_t outLabel = -1) const;

  /* shortcut, inIfIndex = -1, outIfIndex = -1 */
  void InstallEntry (uint32_t inLabel,
                     enum MplsLibEntry::LabelOperation op,
                     uint32_t outLabel = -1) const;

  void Print (void) const;

private:
  Ptr<MplsRoutingProtocol> m_mpls;
};

} // namespace ns3

#endif /* LABEL_INFORMATION_BASE_HELPER_H */
