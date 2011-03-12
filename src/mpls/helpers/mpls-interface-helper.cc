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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/string.h"
#include "ns3/node.h"
#include "ns3/mpls-protocol.h"
#include "ns3/mpls-interface.h"

#include "mpls-interface-helper.h"

NS_LOG_COMPONENT_DEFINE ("MplsInterfaceHelper");

namespace ns3 {

MplsInterfaceHelper::MplsInterfaceHelper ()
{
}

MplsInterfaceHelper::~MplsInterfaceHelper ()
{
}

void 
MplsInterfaceHelper::EnableInterfaceAutoInstall (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      EnableInterfaceAutoInstall (*i);
    }
}

void
MplsInterfaceHelper::EnableInterfaceAutoInstall (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  EnableInterfaceAutoInstall (node);
}


void
MplsInterfaceHelper::EnableInterfaceAutoInstall (Ptr<Node> node) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableInterfaceAutoInstall ();
}

void 
MplsInterfaceHelper::EnableInterfaceAutoInstallAll (void) const
{
  NodeContainer c = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      if ((*i)->GetObject<mpls::MplsProtocol> () != 0)
        {
          EnableInterfaceAutoInstall (*i);
        }
    }
}

void 
MplsInterfaceHelper::DisableInterfaceAutoInstall (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      DisableInterfaceAutoInstall (*i);
    }
}

void
MplsInterfaceHelper::DisableInterfaceAutoInstall (Ptr<Node> node) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->DisableInterfaceAutoInstall ();
}

void
MplsInterfaceHelper::DisableInterfaceAutoInstall (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  DisableInterfaceAutoInstall (node);
}

void 
MplsInterfaceHelper::DisableInterfaceAutoInstallAll (void) const
{
  NodeContainer c = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      if ((*i)->GetObject<mpls::MplsProtocol> () != 0)
        {
          DisableInterfaceAutoInstall (*i);
        }
    }
}

void 
MplsInterfaceHelper::PrintInterfaces (NodeContainer c, std::ostream& os) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      PrintInterfaces (*i, os);
    }
}

void
MplsInterfaceHelper::PrintInterfaces (Ptr<Node> node, std::ostream& os) const
{
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  os << "Node " << node->GetSystemId () << "-" << node->GetId () << " MPLS interfaces:\n";
  os << std::setiosflags(std::ios::right);
  
  for (uint32_t i = 0; i < mpls->GetNInterfaces (); ++i)
    {
      Ptr<mpls::Interface> iface = mpls->GetInterface (i);
      Ptr<NetDevice> dev = iface->GetDevice ();
      os << "  if" << std::setw(10) << i << "dev" << dev->GetIfIndex () << "\n";
    }
}

void
MplsInterfaceHelper::PrintInterfaces (std::string nodeName, std::ostream& os) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  PrintInterfaces (node, os);
}

void 
MplsInterfaceHelper::PrintAllInterfaces (std::ostream& os) const
{
  NodeContainer c = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      if ((*i)->GetObject<mpls::MplsProtocol> () != 0)
        {
          PrintInterfaces (*i, os);
        }
    }
}
} // namespace ns3
