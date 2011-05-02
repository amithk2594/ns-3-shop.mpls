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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/mpls-module.h"
//#include "ns3/ipv4-global-routing-helper.h"

#include <iostream>

using namespace ns3;
using namespace mpls;

int
main (int argc, char *argv[])
{
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("mpls::MplsProtocol", LOG_LEVEL_DEBUG);
  LogComponentEnable ("mpls::Ipv4Routing", LOG_LEVEL_DEBUG);
  LogComponentEnable ("MplsNetworkDiscoverer", LOG_LEVEL_DEBUG);  

  NodeContainer hosts;
  NodeContainer routers;
  NodeContainer routers2;

  PointToPointHelper pointToPoint;
  Ipv4AddressHelper address;
  NetDeviceContainer devices;
  InternetStackHelper internet;
  MplsNetworkConfigurator network;
 
  hosts.Create (2);
  internet.Install (hosts);
  routers = network.CreateAndInstall (3);

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));
  
  // Hosts applications
  uint16_t port = 9;
  UdpEchoServerHelper server (port);
  ApplicationContainer apps = server.Install (hosts.Get(1));
  apps.Start (Seconds (0.01));
  apps.Stop (Seconds (10.0));

  UdpEchoClientHelper client ("192.168.4.2", port);
  client.SetAttribute ("MaxPackets", UintegerValue (5));
  client.SetAttribute ("Interval", TimeValue (MilliSeconds (0.01)));
  client.SetAttribute ("PacketSize", UintegerValue (1024));
  apps = client.Install (hosts.Get (0));
  apps.Start (Seconds (0.01));
  apps.Stop (Seconds (2.0));

  // Hosts configuration
  devices = pointToPoint.Install (hosts.Get(0), routers.Get(0));
  address.SetBase ("192.168.1.0", "255.255.255.0");
  address.Assign (devices);

  devices = pointToPoint.Install (routers.Get(2), hosts.Get(1));
  address.SetBase ("192.168.4.0", "255.255.255.0");
  address.Assign(devices);

  // Routers configuration
  devices = pointToPoint.Install (routers.Get(0), routers.Get(1));
  address.SetBase ("10.1.1.0", "255.255.255.0");
  address.Assign (devices);

  devices = pointToPoint.Install (routers.Get(1), routers.Get(2));
  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (devices);

  // Address is not specified. Mpls interfaces will be disabled.
  devices = pointToPoint.Install (routers.Get(2), routers.Get(0));

  NhlfeSelectionPolicyHelper policy;
  policy.SetAttribute ("MaxPacketsInTxQueue", IntegerValue (0));
  
  MplsSwitch sw1 (routers.Get (0));
  MplsSwitch sw2 (routers.Get (1));
  MplsSwitch sw3 (routers.Get (2));

  sw1.SetSelectionPolicy (policy);

  sw1.AddFtn (Ipv4Source ("192.168.1.1") && Ipv4Destination ("192.168.4.2"),
      Nhlfe (Swap (100), Ipv4Address ("10.1.1.2"))
  );

  sw2.AddIlm (100, 
      Nhlfe(Swap (200), Ipv4Address ("10.1.3.2"))
  );

  sw3.AddIlm (200, 
      Nhlfe(Pop ())
  );


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  network.DiscoverNetwork ();
  network.ShowConfig ();

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
