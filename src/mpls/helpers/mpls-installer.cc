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
#include "ns3/ipv4.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/core-config.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv6-list-routing-helper.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/mpls-protocol.h"

#include "mpls-installer.h"

namespace ns3 {

using namespace mpls;

MplsInstaller::MplsInstaller ()
  : m_routing (0)
{
  SetTcp ("ns3::TcpL4Protocol");
  Ipv4StaticRoutingHelper staticRouting;
  Ipv4GlobalRoutingHelper globalRouting;
  Ipv4ListRoutingHelper listRouting;
  listRouting.Add (staticRouting, 0);
  listRouting.Add (globalRouting, -10);
  SetRoutingHelper (listRouting);
}

MplsInstaller::~MplsInstaller ()
{
  delete m_routing;
}

MplsInstaller::MplsInstaller (const MplsInstaller &o)
{
  m_networkNodes = o.m_networkNodes;
  m_routing = o.m_routing->Copy ();
  m_tcpFactory = o.m_tcpFactory;
}

MplsInstaller&
MplsInstaller::operator= (const MplsInstaller &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_networkNodes = o.m_networkNodes;
  m_routing = o.m_routing->Copy ();
  return *this;
}

const NodeContainer& 
MplsInstaller::GetNetworkNodes (void) const
{
  return m_networkNodes;
}

void
MplsInstaller::SetRoutingHelper (const Ipv4RoutingHelper &routing)
{
  delete m_routing;
  m_routing = routing.Copy ();
}

void
MplsInstaller::SetTcp (const std::string tid)
{
  m_tcpFactory.SetTypeId (tid);
}

void
MplsInstaller::CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
{
  ObjectFactory factory;
  factory.SetTypeId (typeId);
  Ptr<Object> protocol = factory.Create <Object> ();
  node->AggregateObject (protocol);
}

void
MplsInstaller::InstallInternal (Ptr<Node> node)
{
  if (node->GetObject<MplsProtocol> () != 0)
    {
      NS_FATAL_ERROR ("MplsInstaller::Install (): Aggregating "
                      "an Mpls to a node with an existing Mpls object");
      return;
    }

  CreateAndAggregateObjectFromTypeId (node, "ns3::mpls::MplsProtocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::mpls::Ipv4Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::ArpL3Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::Icmpv4L4Protocol");
  CreateAndAggregateObjectFromTypeId (node, "ns3::UdpL4Protocol");

  node->AggregateObject (m_tcpFactory.Create<Object> ());
  Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
  node->AggregateObject (factory);

  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  Ptr<Ipv4RoutingProtocol> ipv4Routing = m_routing->Create (node);
  ipv4->SetRoutingProtocol (ipv4Routing);
  
  m_networkNodes.Add (node);
}

void
MplsInstaller::EnableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableNewInterfaceNotification (true);
}

void
MplsInstaller::DisableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableNewInterfaceNotification (false);
}

}// namespace ns3
