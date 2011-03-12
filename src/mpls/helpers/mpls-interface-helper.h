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
#include "ns3/output-stream-wrapper.h"
#include "ns3/mpls.h"
#include "ns3/ipv4.h"
#include "ns3/ipv6.h"

namespace ns3 {
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
  void EnableInterfaceAutoInstall (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInterfaceHelper::EnableInterfaceAutoInstallInternal, this));
  };

  /**
   * @brief Disable MPLS interfaces auto install onto the provided node.
   *
   * @param node node name, node or container
   */
  template <class T>
  void DisableInterfaceAutoInstall (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInterfaceHelper::DisableInterfaceAutoInstallInternal, this));
  }

  /**
   * @brief Show MPLS interfaces
   *
   * @param node node name, node or container
   * @param os stream to print to
   */
  template <class T>
  void PrintInterfaces (T node) const
  {
    ForEachNode (node, MakeCallback (&MplsInterfaceHelper::PrintInterfacesInternal, this));
  }

  /**
   * Set output stream to print to
   */
  void SetOutputStream (const Ptr<OutputStreamWrapper> &stream);

private:
  void EnableInterfaceAutoInstallInternal (Ptr<Node> node) const;
  void DisableInterfaceAutoInstallInternal (Ptr<Node> node) const;
  void PrintInterfacesInternal (Ptr<Node> node) const;
  void PrintMplsInfo (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Mpls> &mpls) const;
  void PrintIpv4Info (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Ipv4> &ipv4) const;
  void PrintIpv6Info (std::ostream &os, const Ptr<NetDevice> &dev, const Ptr<Ipv6> &ipv6) const;
  void PrintDeviceInfo (std::ostream &os, const Ptr<NetDevice> &dev) const;

  Ptr<OutputStreamWrapper> m_stream;
};

} // namespace ns3

#endif /* MPLS_INTERFACE_HELPER_H */
