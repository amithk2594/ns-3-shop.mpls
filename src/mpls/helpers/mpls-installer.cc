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
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/channel.h"
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

NS_LOG_COMPONENT_DEFINE ("MplsInstaller");

namespace ns3 {

using namespace mpls;

MplsInstaller::MplsInstaller ()
  : m_routing (0)
{
  Initialize ();
}

// private method called by both constructor and Reset ()
void
MplsInstaller::Initialize ()
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
  m_routing = o.m_routing->Copy ();
  m_tcpFactory = o.m_tcpFactory;
}

MplsInstaller &
MplsInstaller::operator = (const MplsInstaller &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_routing = o.m_routing->Copy ();
  return *this;
}

void
MplsInstaller::Reset (void)
{
  delete m_routing;
  m_routing = 0;
  Initialize ();
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
MplsInstaller::InstallInternal (Ptr<Node> node) const
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
}

void
MplsInstaller::PopulateAddressTables (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  NodeList::Iterator listEnd = NodeList::End ();

  for (NodeList::Iterator i = NodeList::Begin (); i != listEnd; i++)
    {
      Ptr<Node> node = *i;
      Ptr<Mpls> mpls = node->GetObject<Mpls> ();

      if (mpls != 0)
        {
          PopulateAddresses (mpls);
        }
    }

}

void
MplsInstaller::PopulateAddresses (const Ptr<Mpls> &mpls)
{
  int32_t nInterfaces = mpls->GetNInterfaces ();

  for (int32_t ifIndex = 0; ifIndex < nInterfaces; ++ifIndex)
    {
      Ptr<Interface> mplsIf = mpls->GetInterface (ifIndex);
      Ptr<NetDevice> device = mplsIf->GetDevice ();
      Ptr<Channel> channel = device->GetChannel ();
      
      mplsIf->RemoveAllAddresses ();

      uint32_t nDevices = channel->GetNDevices ();
      
      for (uint32_t idev = 0; idev < nDevices; ++idev)
        {
          Ptr<NetDevice> dev = channel->GetDevice (idev);
          if (device != dev)
            {
              UpdateInterfaceAddresses (mplsIf, dev);
            }
        }
    }
}

void
MplsInstaller::UpdateInterfaceAddresses (const Ptr<Interface> &interface, const Ptr<NetDevice> &device)
{
  Address addr = device->GetAddress ();
  if (!Mac48Address::IsMatchingType (addr))
    {
      return;
    }
    
  Mac48Address hwaddr = Mac48Address::ConvertFrom (addr);
    
  Ptr<Node> node = device->GetNode ();
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  
  if (mpls == 0)
    {
      return;
    }

  Ptr<Interface> mplsIf = mpls->GetInterfaceForDevice (device);
  
  if (mplsIf == 0)
    {
      return;
    }
    
  Ptr<Ipv4Interface> ipv4If = mplsIf->GetObject<Ipv4Interface> ();

  if (ipv4If == 0) 
    {
      return;
    }
    
  int32_t nAddresses = ipv4If->GetNAddresses ();

  for (int32_t i = 0; i < nAddresses; ++i)
    {
      interface->AddAddress (ipv4If->GetAddress (i).GetLocal (), hwaddr);
    }
}

}// namespace ns3
