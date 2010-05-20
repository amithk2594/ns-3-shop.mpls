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
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-list-routing-helper.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-static-routing.h"

#include "ipv4-to-mpls-routing.h"
#include "ipv6-to-mpls-routing.h"
#include "ip-to-mpls-routing-helper.h"
#include "mpls-stack-helper.h"

NS_LOG_COMPONENT_DEFINE ("MplsStackHelper");

namespace ns3 {

using namespace mpls;

MplsStackHelper::MplsStackHelper ()
  : m_routing (0),
    m_routingv6 (0)
{
  NS_LOG_FUNCTION (this);
  Initialize ();
}

void
MplsStackHelper::Initialize ()
{
  Ipv4ToMplsRoutingHelper mplsRouting;
  m_internetStackHelper.SetRoutingHelper (mplsRouting);

  Ipv6ToMplsRoutingHelper mplsRoutingv6;
  m_internetStackHelper.SetRoutingHelper (mplsRoutingv6);

  SetIpv4Enabled (true);
  SetIpv6Enabled (true);

  Ipv4StaticRoutingHelper staticRouting;
  Ipv4GlobalRoutingHelper globalRouting;
  Ipv4ListRoutingHelper listRouting;
  Ipv6ListRoutingHelper listRoutingv6;
  Ipv6StaticRoutingHelper staticRoutingv6;
  listRouting.Add (staticRouting, 0);
  listRouting.Add (globalRouting, -10);
  listRoutingv6.Add (staticRoutingv6, 0);
  SetRoutingHelper (listRouting);
  SetRoutingHelper (listRoutingv6);
}

MplsStackHelper::~MplsStackHelper ()
{
  delete m_routing;
}

void
MplsStackHelper::SetIpv4Enabled (bool enabled)
{
  m_internetStackHelper.SetIpv4StackInstall (enabled);
  m_ipv4Enabled = enabled;
}

void
MplsStackHelper::SetIpv6Enabled (bool enabled)
{
  m_internetStackHelper.SetIpv6StackInstall (enabled);
  m_ipv6Enabled = enabled;
}

void
MplsStackHelper::SetRoutingHelper (const Ipv4RoutingHelper &routing)
{
  delete m_routing;
  m_routing = routing.Copy ();
}

void
MplsStackHelper::SetRoutingHelper (const Ipv6RoutingHelper &routing)
{
  delete m_routingv6;
  m_routingv6 = routing.Copy ();
}

void
MplsStackHelper::Install (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node);

  if (node->GetObject<MplsRoutingProtocol> () != 0)
    {
      NS_FATAL_ERROR ("MplsStackHelper::Install (): "
                      "Aggregating an MplsRoutingProtocol to a node with an existing MplsRoutingProtocol object");
      return;
    }

  NS_LOG_LOGIC ("MplsStackHelper::Install (): Aggregate MplsRoutingProtocol");

  Ptr<MplsRoutingProtocol> mpls = CreateObject<MplsRoutingProtocol> ();
  node->AggregateObject (mpls);
  m_internetStackHelper.Install (node);
  if (m_ipv4Enabled)
    {
      Ptr<Ipv4ToMplsRouting> routing = GetIpv4ToMplsRouting (node);
      NS_ASSERT_MSG (routing != 0, "MplsStackHelper::Install (): bad routing");
      routing->SetRoutingProtocol (m_routing->Create (node));
    }
  if (m_ipv6Enabled)
    {
      Ptr<Ipv6ToMplsRouting> routingv6 = GetIpv6ToMplsRouting (node);
      NS_ASSERT_MSG (routingv6 != 0, "MplsStackHelper::Install (): bad routing");
      routingv6->SetRoutingProtocol (m_routingv6->Create (node));
    }
}

void
MplsStackHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
MplsStackHelper::Assign (const NetDeviceContainer &c) const
{
  for (uint32_t i = 0; i < c.GetN (); ++i)
    {
      Ptr<NetDevice> device = c.Get (i);
      Assign (device);
    }
}

void
MplsStackHelper::Assign (Ptr<NetDevice> device) const
{
  Ptr<Node> node = device->GetNode ();
  NS_ASSERT_MSG (node != 0, "MplsStackHelper::ConfigureInterface (): Bad node");

  Ptr<MplsRoutingProtocol> mpls = node->GetObject<MplsRoutingProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsStackHelper::ConfigureInterface (): Bad MplsRoutingProtocol");

  if (mpls->GetMplsDeviceIndex (device) < 0)
    {
      mpls->AddInterface (device);
    }

/*  if (m_ipv4Enabled)
    {
      SetIpv4AllRoutersMulticast (node, mpls);
    }*/
}

Ptr<Ipv4ToMplsRouting>
MplsStackHelper::GetIpv4ToMplsRouting (Ptr<Node> node) const
{
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  NS_ASSERT_MSG (ipv4 != 0, "MplsStackHelper::GetIpv4ToMplsRouting (): bad ipv4");

  return DynamicCast<Ipv4ToMplsRouting> (ipv4->GetRoutingProtocol ());
}

Ptr<Ipv6ToMplsRouting>
MplsStackHelper::GetIpv6ToMplsRouting (Ptr<Node> node) const
{
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  NS_ASSERT_MSG (ipv6 != 0, "MplsStackHelper::GetIpv6ToMplsRouting (): bad ipv6");

  return DynamicCast<Ipv6ToMplsRouting> (ipv6->GetRoutingProtocol ());
}

Ptr<Ipv4StaticRouting>
MplsStackHelper::GetStaticRouting (Ptr<Node> node) const
{
  // XXX: fix in future, not flexible
  Ptr<Ipv4ToMplsRouting> routing = GetIpv4ToMplsRouting (node);
  NS_ASSERT_MSG (routing != 0, "MplsStackHelper::GetStaticRouting (): bad routing");

  Ptr<Ipv4StaticRouting> staticRouting = DynamicCast<Ipv4StaticRouting> (routing->GetRoutingProtocol ());
  if (staticRouting != 0)
    {
      return staticRouting;
    }

  Ptr<Ipv4ListRouting> listRouting = DynamicCast<Ipv4ListRouting> (routing->GetRoutingProtocol ());
  if (listRouting == 0)
    {
      return 0;
    }

  uint32_t i = 0;
  int16_t priority;
  while (i < listRouting->GetNRoutingProtocols ())
    {
      staticRouting = DynamicCast<Ipv4StaticRouting> (listRouting->GetRoutingProtocol (i, priority));
      if (staticRouting != 0)
        {
          return staticRouting;
        }
      ++i;
    }

  return 0;
}

/*
void
MplsStackHelper::SetIpv4AllRoutersMulticast (Ptr<Node> node, Ptr<MplsRoutingProtocol> mpls) const
{
  Ptr<Ipv4StaticRouting> routing = GetStaticRouting (node);

  NS_ASSERT_MSG (routing != 0, "MplsStackHelper::AllRoutersMulticast (): bad routing");

  routing->RemoveMulticastRoute (Ipv4Address::GetAny(), Ipv4Address("224.0.0.2"), Ipv4::IF_ANY);

  std::list<Ptr<NetDevice> > devices = mpls->GetMplsDeviceList ();
  if (devices.size () > 0)
  {
    std::vector<uint32_t> indexes;
    for (std::list<Ptr<NetDevice> >::const_iterator i = devices.begin (); i != devices.end (); ++i)
      {
        indexes.push_back ((*i)->GetIfIndex ());
      }

    routing->AddMulticastRoute (Ipv4Address::GetAny(), Ipv4Address("224.0.0.2"), Ipv4::IF_ANY, indexes);
  }
}
*/

} // namespace ns3
