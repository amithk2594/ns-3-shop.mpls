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

#ifndef MPLS_NETWORK_CONFIGURATOR_H
#define MPLS_NETWORK_CONFIGURATOR_H

#include "ns3/mpls-installer.h"
#include "ns3/mpls-interface-helper.h"
#include "ns3/mpls-network-discoverer.h"
#include "ns3/mpls-tunnel-helper.h"

namespace ns3 {

/**
 * \brief Mpls network configurator
 */
class MplsNetworkConfigurator : public MplsInstaller, public MplsNetworkDiscoverer, public MplsTunnelHelper,
        public MplsInterfaceHelper
{
public:
  /**
   * @brief Create mpls network configurator
   */
  MplsNetworkConfigurator();
  /**
   * @brief Destroy object
   */
  virtual ~MplsNetworkConfigurator(void);

  MplsNetworkConfigurator (const MplsNetworkConfigurator &o);
  MplsNetworkConfigurator& operator= (const MplsNetworkConfigurator &o);

  void SetOutputStream (const Ptr<OutputStreamWrapper> &stream);
  virtual const Ptr<OutputStreamWrapper>& GetOutputStream (void) const;
  virtual const NodeContainer& GetNetworkNodes (void) const;
  
private:
  Ptr<OutputStreamWrapper> m_stream;
};

} // namespace ns3

#endif /* MPLS_NETWORK_CONFIGURATOR_H */
