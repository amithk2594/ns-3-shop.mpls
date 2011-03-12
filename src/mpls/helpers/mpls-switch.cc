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

MplsSwitch::MplsSwitch ()
{
}

MplsSwitch::MplsSwitch (const Ptr<Node> &node)
  : MplsFtnHelper (node), MplsIlmHelper (node)
{
}

MplsSwitch::MplsSwitch (const std::string &node)
  : MplsFtnHelper (node), MplsIlmHelper (node)
{
}

MplsSwitch::~MplsSwitch()
{
}

void
MplsSwitch::AddInterface (uint32_t devIfIndex)
{
  Ptr<NetDevice> dev = MplsFtnHelper::GetNode ()->GetDevice (devIfIndex);
  NS_ASSERT_MSG (dev != 0, "MplsSwitch::AddInterface (): Bad device");
  NS_ASSERT_MSG (MplsFtnHelper::GetMpls ()->GetInterfaceForDevice (dev) == 0, 
                  "MplsSwitch::AddInterface (): Interface for device already added");
  MplsFtnHelper::GetMpls ()->AddInterface (dev);
}

bool
MplsSwitch::IsUp (uint32_t i) const
{
  Ptr<mpls::Interface> iface = MplsFtnHelper::GetMpls ()->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->IsUp ();
}

void
MplsSwitch::SetUp (uint32_t i)
{
  Ptr<mpls::Interface> iface = MplsFtnHelper::GetMpls ()->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->SetUp ();
}

void
MplsSwitch::SetDown (uint32_t i)
{
  Ptr<mpls::Interface> iface = MplsFtnHelper::GetMpls ()->GetInterface (i);
  NS_ASSERT_MSG (iface != 0, "MplsSwitch::IsUp (): Bad index of interface");
  return iface->SetDown ();
}

} // namespace mpls
