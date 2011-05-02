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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/string.h"

#include "mpls-switch.h"

NS_LOG_COMPONENT_DEFINE ("MplsSwitch");

namespace ns3 {

MplsSwitch::MplsSwitch (const Ptr<Node> &node)
  : m_nhlfeSelectionPolicy ()
{
  m_node = DynamicCast<MplsNode> (node);
  NS_ASSERT_MSG (m_node != 0, "Possible you use Node instead of MplsNode");
  
  m_mpls = node->GetObject<Mpls> ();
  
  NS_ASSERT_MSG (m_mpls != 0, "There is no mpls installed on specified node");
}

MplsSwitch::MplsSwitch (const std::string &node)
{
  MplsSwitch (Names::Find<MplsNode> (node));
}

MplsSwitch::~MplsSwitch ()
{
}

MplsSwitch::MplsSwitch (const MplsSwitch &o)
{
  m_mpls = o.m_mpls;
  m_node = o.m_node;
  m_nhlfeSelectionPolicy = o.m_nhlfeSelectionPolicy;
}

MplsSwitch& 
MplsSwitch::operator= (const MplsSwitch &o)
{
  if (this == &o)
    {
      return *this;
    }

  m_mpls = o.m_mpls;
  m_node = o.m_node;
  m_nhlfeSelectionPolicy = o.m_nhlfeSelectionPolicy;
  return *this;
}

Ptr<mpls::Interface>
MplsSwitch::AddInterface (uint32_t devIfIndex)
{
  Ptr<NetDevice> dev = m_node->GetDevice (devIfIndex);
  NS_ASSERT_MSG (dev != 0, "MplsSwitch::AddInterface (): Bad device");
  NS_ASSERT_MSG (m_mpls->GetInterfaceForDevice (dev) == 0, 
                  "MplsSwitch::AddInterface (): Interface for device already added");
  return m_mpls->AddInterface (dev);
}

Ptr<mpls::Interface>
MplsSwitch::GetInterface (uint32_t i)
{
  return m_mpls->GetInterface (i);
}

bool
MplsSwitch::IsUp (uint32_t i) const
{
  Ptr<mpls::Interface> iface = m_mpls->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->IsUp ();
}

void
MplsSwitch::SetUp (uint32_t i)
{
  Ptr<mpls::Interface> iface = m_mpls->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->SetUp ();
}

void
MplsSwitch::SetDown (uint32_t i)
{
  Ptr<mpls::Interface> iface = m_mpls->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->SetDown ();
}

const Ptr<Mpls>&
MplsSwitch::GetMpls (void) const
{
  return m_mpls;
}

const Ptr<MplsNode>&
MplsSwitch::GetNode (void) const
{
  return m_node;
}

void
MplsSwitch::SetSelectionPolicy(const NhlfeSelectionPolicyHelper& policy)
{
  m_nhlfeSelectionPolicy = policy;
}

const NhlfeSelectionPolicyHelper&
MplsSwitch::GetSelectionPolicy (void) const
{
  return m_nhlfeSelectionPolicy;
}

} // namespace mpls
