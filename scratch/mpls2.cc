/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mpls-module.h"

#include <iostream>

using namespace ns3;
using namespace mpls;

void
setLocation(Ptr<Node> node, double x, double y)
{
  Ptr<CanvasLocation> loc = node->GetObject<CanvasLocation> ();
  if (loc == 0)
    {
      loc = CreateObject<CanvasLocation> ();
      node->AggregateObject (loc);
    }
  Vector vec (x, y, 0);
  loc->SetLocation (vec);
}
  
int 
main (int argc, char *argv[])
{
 
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable("MplsRoutingProtocol", LOG_LEVEL_DEBUG);
  LogComponentEnable("MplsLabelStack", LOG_LEVEL_DEBUG);  

  // Create hosts
  NodeContainer hosts;
  hosts.Create (4);  

  // Create mpls routers
  NodeContainer routers;
  routers.Create (3);
  
  // Configure hosts
  InternetStackHelper internet;
  internet.Install (hosts);

  // Configure routers
  MplsStackHelper mpls;
  mpls.Install (routers);
  
  // Mpls routers configuration
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));

  Ipv4AddressHelper address;
  NetDeviceContainer devices;

  devices = pointToPoint.Install (routers.Get(0), routers.Get(1));
  // Configure Ipv4 interfaces
  address.SetBase ("10.1.1.0", "255.255.255.0");
  address.Assign (devices);
  // Configure Mpls interface
  mpls.Assign (devices);

  devices = pointToPoint.Install (routers.Get(1), routers.Get(2));
  // Configure Ipv4 interfaces   
  address.SetBase ("10.1.2.0", "255.255.255.0");
  address.Assign (devices);
  // Configure Mpls interface  
  mpls.Assign (devices);   

  // Hosts configuration
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("4ms"));
    
  devices = pointToPoint.Install (routers.Get(0), hosts.Get(0));
  address.SetBase ("192.168.1.0", "255.255.255.0");
  address.Assign(devices);
  devices = pointToPoint.Install (routers.Get(0), hosts.Get(1));
  address.SetBase ("192.168.2.0", "255.255.255.0");
  address.Assign(devices);
  devices = pointToPoint.Install (routers.Get(2), hosts.Get(2));
  address.SetBase ("192.168.3.0", "255.255.255.0");
  address.Assign(devices);
  devices = pointToPoint.Install (routers.Get(2), hosts.Get(3));
  address.SetBase ("192.168.4.0", "255.255.255.0");
  address.Assign(devices);

  // Animation settings
  setLocation(hosts.Get(0), 0, -0.2);
  setLocation(hosts.Get(1), 0, 0.2);
  setLocation(routers.Get(0), 1, 0);
  setLocation(routers.Get(1), 3, 0);
  setLocation(routers.Get(2), 5, 0);
  setLocation(hosts.Get(2), 6, -0.2);
  setLocation(hosts.Get(3), 6, 0.2);
    
  // LIB table for router 0
  LabelInformationBaseHelper libtable;  

  libtable.SetNode (routers.Get(0));
  /*----------------------------------------------------------------------------------------------*/    
  /* Lsr-edge 0          InIf               FEC / InLabel         OutIf    Operation     OutLabel */
  /*----------------------------------------------------------------------------------------------*/  
  libtable.InstallEntry (     Ipv4AddressPrefix("192.168.4.0/24"),  1,                         20 );
  libtable.InstallEntry (  1,                                  18, -1,  MplsLibEntry::POP         );
  libtable.InstallEntry ( -1,                                  16, -1,  MplsLibEntry::POP         );
  libtable.Print ();
  
  libtable.SetNode (routers.Get(1));
  /*----------------------------------------------------------------------------------------------*/    
  /* Lsr-core 1          InIf               FEC / InLabel         OutIf     Operation    OutLabel */
  /*----------------------------------------------------------------------------------------------*/
  libtable.InstallEntry (  1,                                  20,  2,  MplsLibEntry::SWAP,    25 );
  libtable.InstallEntry (  2,                                  17,  1,  MplsLibEntry::SWAP,    18 );
  libtable.Print ();


  libtable.SetNode (routers.Get(2));
  /*----------------------------------------------------------------------------------------------*/    
  /* Lsr-edge 2          InIf               FEC / InLabel         OutIf     Operation    OutLabel */
  /*----------------------------------------------------------------------------------------------*/
  libtable.InstallEntry (     Ipv4AddressPrefix("192.168.1.0/24"),                             16 );
  libtable.InstallEntry ( -1,                                  16,  1,  MplsLibEntry::PUSH,    17 );
  libtable.InstallEntry (  1,                                  25,  2,  MplsLibEntry::POP         );
  
  libtable.Print();  
  
  // Animation settings

  // Hosts applications
  uint16_t port = 9;
  UdpEchoServerHelper server (port);
  ApplicationContainer apps = server.Install (hosts.Get(3));
  apps.Start (Seconds (0.01));
  apps.Stop (Seconds (2.0));
  
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client ("192.168.4.2", port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps = client.Install (hosts.Get (0));
  apps.Start (Seconds (0.01));
  apps.Stop (Seconds (2.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//  // Simulation 
//  AnimationInterface anim;
//  anim.SetOutputFile ("anim.nam");
//  anim.StartAnimation ();
  
  Simulator::Run ();
  Simulator::Destroy ();

//  anim.StopAnimation();

  return 0;
}
