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

#include <iostream>
#include <iomanip>
#include "ns3/log.h"
#include "ns3/assert.h"

#include "mpls-label-stack.h"
#include "label-information-base-helper.h"

NS_LOG_COMPONENT_DEFINE ("LabelInformationBaseHelper");

namespace ns3 {

using namespace mpls;

LabelInformationBaseHelper::LabelInformationBaseHelper ()
  : m_mpls (0)
{
}

LabelInformationBaseHelper::LabelInformationBaseHelper (Ptr<Node> node)
{
  SetNode (node);
}

void
LabelInformationBaseHelper::SetNode (Ptr<Node> node)
{
  NS_ASSERT (node != 0);
  Ptr<MplsRoutingProtocol> mpls = node->GetObject<MplsRoutingProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "LabelInformationBaseHelper::LabelInformationBaseHelper (): Bad MplsRoutingProtocol");
  m_mpls = mpls;
}

LabelInformationBaseHelper::~LabelInformationBaseHelper ()
{
  m_mpls = 0;
}

void
LabelInformationBaseHelper::InstallEntry (
  const ForwardingEquivalenceClass &fec,
  int32_t outIfIndex,
  uint32_t outLabel) const
{
  NS_ASSERT_MSG (m_mpls != 0, "LabelInformationBaseHelper::LabelInformationBaseHelper (): Bad MplsRoutingProtocol");

  NS_ASSERT_MSG (outIfIndex == -1 || m_mpls->GetDevice (outIfIndex),
                 "LabelInformationBaseHelper::InstallEntry (): Malformed LIB entry -- bad outgoing interface");

  NS_ASSERT_MSG (outLabel <= MplsLabelStackEntry::MPLS_LABEL_MAX, "LabelInformationBaseHelper::InstallEntry (): "
                 "Malformed LIB entry -- bad outgoing label");

  Ptr<MplsLibEntry> entry = Create<MplsLibEntry> ();
  entry->SetFec (fec);
  entry->SetOutIfIndex (outIfIndex);
  entry->SetOperation (MplsLibEntry::PUSH);
  entry->SetOutLabel (outLabel);

  m_mpls->AddLibEntry (entry);
}

void
LabelInformationBaseHelper::InstallEntry (const ForwardingEquivalenceClass &fec, uint32_t outLabel) const
{
  InstallEntry (fec, -1, outLabel);
}

void
LabelInformationBaseHelper::InstallEntry (int32_t inIfIndex, uint32_t inLabel, int32_t outIfIndex,
                                          enum MplsLibEntry::LabelOperation op,  uint32_t outLabel) const
{
  NS_ASSERT_MSG (m_mpls != 0, "LabelInformationBaseHelper::LabelInformationBaseHelper (): Bad MplsRoutingProtocol");

  NS_ASSERT_MSG (inIfIndex == -1 || m_mpls->GetMplsDevice (inIfIndex),
                 "LabelInformationBaseHelper::InstallEntry (): Malformed LFIB entry -- bad incomming interface");

  NS_ASSERT_MSG (inLabel <= MplsLabelStackEntry::MPLS_LABEL_MAX,
                 "LabelInformationBaseHelper::InstallEntry (): Malformed LFIB entry -- bad incomming label");

  NS_ASSERT_MSG (outIfIndex == -1 || m_mpls->GetDevice (outIfIndex),
                 "LabelInformationBaseHelper::InstallEntry (): Malformed LFIB entry -- bad outgoing interface");

  NS_ASSERT_MSG (outLabel <= MplsLabelStackEntry::MPLS_LABEL_MAX || (op == MplsLibEntry::POP && outLabel == (uint32_t)-1),
                 "LabelInformationBaseHelper::InstallEntry (): Malformed LFIB entry -- bad outcomming label");

  Ptr<MplsLibEntry> entry = Create<MplsLibEntry> ();

  entry->SetInIfIndex (inIfIndex);
  entry->SetInLabel (inLabel);
  entry->SetOutIfIndex (outIfIndex);
  entry->SetOperation (op);
  entry->SetOutLabel (outLabel);

  m_mpls->AddLibEntry (entry);
}

void
LabelInformationBaseHelper::InstallEntry (
  uint32_t inLabel,
  enum MplsLibEntry::LabelOperation op,
  uint32_t outLabel) const
{
  InstallEntry (-1, inLabel, -1, op, outLabel);
}

void
LabelInformationBaseHelper::Print (void) const
{
  NS_ASSERT_MSG (m_mpls != 0, "LabelInformationBaseHelper::LabelInformationBaseHelper (): Bad MplsRoutingProtocol");

  std::cout << "Mpls Label Information Base for node " << m_mpls->GetObject<Node> ()->GetId () << std::endl;
  std::cout << std::setiosflags (std::ios::left);
  std::cout << std::setw (6) << "InIf";
  std::cout << std::setw (30) << "FEC / InLabel";
  std::cout << std::setw (6) << "OutIf";
  std::cout << std::setw (6) << "Op";
  std::cout << std::setw (9) << "OutLabel" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;
  m_mpls->PrintLibEntries (std::cout);
  std::cout << std::endl;
}

} // namespace ns3
