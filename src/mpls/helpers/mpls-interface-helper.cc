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
#include "ns3/names.h"
#include "ns3/string.h"
#include "ns3/mpls-interface.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/mac48-address.h"

#include "mpls-interface-helper.h"
#include "mpls-node-helper.h"


NS_LOG_COMPONENT_DEFINE ("MplsInterfaceHelper");

namespace ns3 {

MplsInterfaceHelper::MplsInterfaceHelper ()
{
  m_stream = Create<OutputStreamWrapper> (&std::cout);
}

MplsInterfaceHelper::MplsInterfaceHelper (const Ptr<OutputStreamWrapper> &stream)
  : m_stream (stream)
{
}

MplsInterfaceHelper::~MplsInterfaceHelper ()
{
  m_stream = 0;
}

void
MplsInterfaceHelper::SetOutputStream (const Ptr<OutputStreamWrapper> &stream)
{
  NS_ASSERT_MSG (stream != 0, "MplsInterfaceHelper::SetOutputStream (): invalid stream");
  m_stream = stream;
}

void
MplsInterfaceHelper::EnableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableNewInterfaceNotification (true);
}

void
MplsInterfaceHelper::DisableInterfaceAutoInstallInternal (Ptr<Node> node) const
{
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");
  mpls->EnableNewInterfaceNotification (false);
}

void
MplsInterfaceHelper::PrintInterfacesInternal (Ptr<Node> node) const
{
  std::ostream &os = *(m_stream->GetStream ());

  Ptr<Mpls> mpls = node->GetObject<Mpls> ();
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();

  os << "Node " << node->GetId () << " interfaces:" << std::endl;
  os << std::setiosflags(std::ios::left);

  for (uint32_t i = 0, c = node->GetNDevices (); i < c; ++i)
    {
      Ptr<NetDevice> dev = node->GetDevice (i);
      Address hwaddr = dev->GetAddress ();

      os << "dev" << std::setw (5) << i << " "
         << "Type " << dev->GetInstanceTypeId ().GetName () << "  "
         << "HWaddr ";


      if (Mac48Address::IsMatchingType (hwaddr))
        {
          os << Mac48Address::ConvertFrom (hwaddr);
        }
      else
        {
          os << hwaddr;
        }
        
      os << std::endl;

      if (mpls != 0)
        {
          PrintMplsInfo (os, dev, mpls);
        }

      if (ipv4 != 0)
        { 
          PrintIpv4Info (os, dev, ipv4);
        }
        
      PrintDeviceInfo (os, dev);
      os << std::endl;
    }
}

void
MplsInterfaceHelper::PrintMplsInfo (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Mpls> &mpls) const
{
  Ptr<mpls::Interface> iface = mpls->GetInterfaceForDevice (dev);
  if (iface != 0) 
    {
      os << std::setw (9) << " " 
         << "mpls:" << (iface->IsUp () ? "enabled" : "disabled") << "  "
         << "Interface: " << iface->GetIfIndex ()
         << std::endl;
    }  
}

void
MplsInterfaceHelper::PrintIpv4Info (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Ipv4> &ipv4) const
{
  int32_t ifIndex = ipv4->GetInterfaceForDevice (dev);
  if (ifIndex >= 0)
    {
      for (uint32_t i = 0, c = ipv4->GetNAddresses (ifIndex); i < c; ++i)
        {
          Ipv4InterfaceAddress addr = ipv4->GetAddress (ifIndex, i);
          os << std::setw (9) << " " 
             << "inet addr:" << addr.GetLocal () << "  "
             << "Bcast:" << addr.GetBroadcast () << "  "
             << "Mask:" << addr.GetMask ();

          if (addr.IsSecondary ())
            {
              os << "  (secondary)";
            }
          os << std::endl;
        }
    }
}

void
MplsInterfaceHelper::PrintIpv6Info (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Ipv6> &ipv6) const
{
}

void
MplsInterfaceHelper::PrintDeviceInfo (std::ostream &os, const Ptr<NetDevice> &dev) const
{
  os << std::setw (9) << " " 
     << (dev->IsLinkUp () ? "UP" : "DOWN");

  if (dev->IsPointToPoint ())
    {
      os << " PPP";
    }

  if (dev->IsBridge ())
    {
      os << " BRIDGE";
    }

  if (dev->IsMulticast ())
    {
      os << " MULTICAST";
    }

  if (dev->IsBroadcast ())
    {
      os << " BROADCAST";
    }
    
  if (dev->NeedsArp ())
    {
      os << " ARP";
    }

  os << "  MTU:" << dev->GetMtu ();
  os << std::endl;
}

} // namespace ns3
