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

#include "mpls-network-configurator.h"

NS_LOG_COMPONENT_DEFINE ("MplsNetworkConfigurator");

namespace ns3 {

using namespace mpls;

MplsNetworkConfigurator::MplsNetworkConfigurator ()
{
  m_stream = Create<OutputStreamWrapper> (&std::cout);
}

MplsNetworkConfigurator::~MplsNetworkConfigurator ()
{
}

MplsNetworkConfigurator::MplsNetworkConfigurator (const MplsNetworkConfigurator &o)
  : MplsInstaller (o), 
    MplsNetworkDiscoverer (o),
    MplsTunnelHelper (o)
{
  m_stream = o.m_stream;
}

MplsNetworkConfigurator&
MplsNetworkConfigurator::operator= (const MplsNetworkConfigurator &o)
{
  if (this == &o)
    {
      return *this;
    }

  MplsInstaller::operator= (o);
  MplsTunnelHelper::operator= (o);
  MplsNetworkDiscoverer::operator= (o);

  m_stream = o.m_stream;
  
  return *this;
}

void
MplsNetworkConfigurator::SetOutputStream (const Ptr<OutputStreamWrapper> &stream)
{
  NS_ASSERT_MSG (stream != 0, "MplsNetworkConfigurator::SetOutputStream (): invalid stream");
  m_stream = stream;
}

const NodeContainer& 
MplsNetworkConfigurator::GetNetworkNodes (void) const
{
  return MplsInstaller::GetNetworkNodes ();
}

const Ptr<OutputStreamWrapper>&
MplsNetworkConfigurator::GetOutputStream (void) const
{
  return m_stream;
}

}// namespace ns3
