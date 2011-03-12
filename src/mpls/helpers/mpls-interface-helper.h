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

#ifndef MPLS_INTERFACE_HELPER_H
#define MPLS_INTERFACE_HELPER_H

#include <ostream>

#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/ptr.h"

namespace ns3 {

class Node;

/**
 * \brief Mpls interface helper
 */
class MplsInterfaceHelper
{
public:
  MplsInterfaceHelper ();
  MplsInterfaceHelper (const Ptr<OutputStreamWrapper> &stream);
  virtual ~MplsInterfaceHelper ();

  /**
   * @brief Enable MPLS interfaces auto install onto the provided node.
   *
   * @param node node name, node or container
   */
  template <class T>
  void EnableInterfaceAutoInstall (T node) const;

  /**
   * @brief Disable MPLS interfaces auto install onto the provided node.
   *
   * @param node node name, node or container
   */
  template <class T>
  void DisableInterfaceAutoInstall (T node) const;

  /**
   * @brief Show MPLS interfaces
   *
   * @param node node name, node or container
   * @param os stream to print to
   */
  template <class T>
  void PrintInterfaces (T node) const;

  /**
   * Set output stream to print to
   */
  void SetOutputStream (const Ptr<OutputStreamWrapper> &stream);

private:
  void EnableInterfaceAutoInstallInternal (Ptr<Node> node) const;
  void DisableInterfaceAutoInstallInternal (Ptr<Node> node) const;
  void PrintInterfacesInternal (Ptr<Node> node) const;
  void PrintDeviceInformation (std::ostream &os, Ptr<NetDevice> device) const;

  Ptr<OutputStreamWrapper> m_stream;
};

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
  Ptr<mpls::MplsProtocol> mpls = node->GetObject<mpls::MplsProtocol> ();
  NS_ASSERT_MSG (mpls != 0, "MplsInterfaceHelper::DisableInterfaceAutoInstall (): Install MPLS first");

  //std::ostream *os = m_stream->GetStream ();

  //std::cout << &std::cout << " " << os << std::endl;
//  os << "Node " << node->GetSystemId () << "-" << node->GetId () << " MPLS interfaces:" << std::endl;
//  os << std::setiosflags(std::ios::left);

//  for (uint32_t i = 0; i < mpls->GetNInterfaces (); ++i)
//    {
//      Ptr<mpls::Interface> iface = mpls->GetInterface (i);
//      Ptr<NetDevice> dev = iface->GetDevice ();
//      os << "  if" << std::setw(10) << i << "dev" << std::setw(5) << dev->GetIfIndex ();
//      PrintDeviceInformation (os, dev);
//      os << std::endl;
//    }
}

void
MplsInterfaceHelper::PrintDeviceInformation (std::ostream &os, Ptr<NetDevice> device) const
{
  Address hwaddr = device->GetAddress ();

  os << "HWaddr "
     << Mac48Address::IsMatchingType (hwaddr) ? Mac48Address::ConvertFrom (hwaddr) : hwaddr;
}

} // namespace ns3

#endif /* MPLS_INTERFACE_HELPER_H */
