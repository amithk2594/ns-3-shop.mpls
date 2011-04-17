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
#include "ns3/channel.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-interface.h"
#include "ns3/mpls-protocol.h"

#include "mpls-mac-resolver.h"

NS_LOG_COMPONENT_DEFINE ("MplsMacResolver");

namespace ns3 {

using namespace mpls;

MplsMacResolver::MplsMacResolver ()
{
}

MplsMacResolver::~MplsMacResolver ()
{
}

void
MplsMacResolver::ConfigureMacResolvers (void)
{
  const NodeContainer& nodes = GetNetworkNodes ();

  for (NodeContainer::Iterator i = nodes.Begin (), j = nodes.End (); i != j; ++i)
    {
      UpdateNodeAddresses (*i);
    }
}

void
MplsMacResolver::UpdateNodeAddresses (const Ptr<Node> &node)
{
  NS_LOG_DEBUG ("[node " << node->GetId() << "] configuring mac resolver");

  Ptr<Mpls> mpls = node->GetObject<Mpls> ();

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
MplsMacResolver::UpdateInterfaceAddresses (const Ptr<Interface> &interface, const Ptr<NetDevice> &device)
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
      Ipv4Address addr = ipv4If->GetAddress (i).GetLocal ();
      interface->AddAddress (addr, hwaddr);
      NS_LOG_DEBUG ("[node " << node->GetId() << "] adding explicitly defined address " << hwaddr << " for " << addr);
    }
}

}// namespace ns3
