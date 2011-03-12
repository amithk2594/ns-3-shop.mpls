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
  std::ostream &os = *(m_stream->GetStream ());

  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();

  os << "Node " << node->GetSystemId () << "-" << node->GetId () << " interfaces:" << std::endl;
  os << std::setiosflags(std::ios::left);

  for (uint32_t i = 0, c = node->GetNDevices (); i < c; ++i)
    {
      Address hwaddr = device->GetAddress ();

      os << "dev" << std::setw (5) << i << " "
         << "Type " << device->GetInstanceTypeId ().GetName () << "  "
         << "HWaddr ";


      if (Mac48Address::IsMatchingType (hwaddr))
        {
          os << Mac48Address::ConvertFrom (hwaddr);
        }
      else
        {
          os << hwaddr;
        }

      //Ptr<mpls::Interface> iface = mpls->GetInterface (i);
      //Ptr<NetDevice> dev = iface->GetDevice ();

      //if (ipv4)
      //  {
      //    PrintIpv4Information (os, ipv4, device, 23);
      //  }
      //PrintDeviceInformation (os, device, 23);

      os << std::endl;
    }
}

void
MplsInterfaceHelper::PrintMplsInformation (std::ostream &os, const Ptr<MplsProtocol> &mpls,
    const Ptr<NetDevice> &device,  uint32_t indent) const
{
}

void
MplsInterfaceHelper::PrintIpv4Information (std::ostream &os, const Ptr<Ipv4> &ipv4,
    const Ptr<NetDevice> &device,  uint32_t indent) const
{
  int32_t ifIndex = ipv4->GetInterfaceForDevice (dev);
  if (ifIndex < 0) return;

  os << std::setw (indent) << " ";

//  for (uint32_t i = 0, c = ipv4->GetNAddresses (ifIndex); i < c; ++i)
//    {
//      Ipv4InterfaceAddress addr = ipv4->GetAddress (ifIndex, i);
//      Ipv4Address local = addr.GetLocal ();
//      Ipv4Mask mask = addr.GetMask ();
//    }
}

void
MplsInterfaceHelper::PrintDeviceInformation (std::ostream &os, const Ptr<NetDevice> &device,
     uint32_t indent) const
{
  os << std::setw (indent) << " ";

  os << (device->IsLinkUp () ? "UP" : "DOWN");

  if (device->IsPointToPoint ())
    {
      os << " PPP";
    }

  if (device->IsBridge ())
    {
      os << " BRIDGE";
    }

  if (device->IsMulticast ())
    {
      os << " MULTICAST";
    }

  if (device->IsBroadcast ())
    {
      os << " BROADCAST";
    }

  os << "  MTU:" << device->GetMtu ();
  os << std::endl;
}

} // namespace ns3
