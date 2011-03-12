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

#include <iomanip>
#include <ostream>

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/string.h"
#include "ns3/node.h"
#include "ns3/mpls-protocol.h"
#include "ns3/mpls-interface.h"

#include "mpls-interface-helper.h"
#include "mpls-node-helper.h"


NS_LOG_COMPONENT_DEFINE ("MplsInterfaceHelper");

namespace ns3 {

MplsInterfaceHelper::MplsInterfaceHelper ()
{
}

MplsInterfaceHelper::~MplsInterfaceHelper ()
{
}

template <class T>
void
MplsInterfaceHelper::EnableInterfaceAutoInstall (T node) const
{
  ForEachNode (node, MakeCallback (&MplsInterfaceHelper::EnableInterfaceAutoInstallInternal, this));
}

template <class T>
void
MplsInterfaceHelper::DisableInterfaceAutoInstall (T node) const
{
  ForEachNode (node, MakeCallback (&MplsInterfaceHelper::DisableInterfaceAutoInstallInternal, this));
}

template <class T>
void
MplsInterfaceHelper::PrintInterfaces (T node) const
{
  ForEachNode (node, MakeCallback (&MplsInterfaceHelper::PrintInterfacesInternal, this));
}

void
MplsInterfaceHelper::EnableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableInterfaceAutoInstall ();
}

void
MplsInterfaceHelper::DisableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->DisableInterfaceAutoInstall ();
}

void
MplsInterfaceHelper::PrintInterfacesInternal (Ptr<Node> node) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  std::cout << "Node " << node->GetSystemId () << "-" << node->GetId () << " MPLS interfaces:\n";
  std::cout << std::setiosflags(std::ios::left);

  for (uint32_t i = 0; i < mpls->GetNInterfaces (); ++i)
    {
      Ptr<mpls::Interface> iface = mpls->GetInterface (i);
      Ptr<NetDevice> dev = iface->GetDevice ();
      std::cout << "  if" << std::setw(10) << i << "dev" << dev->GetIfIndex () << "\n";
    }
}

} // namespace ns3
