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
 
#include "mpls-ipv4-protocol.h"
#include "mpls-ipv4-routing.h"

NS_LOG_COMPONENT_DEFINE ("MplsIpv4Protocol");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (MplsIpv4Protocol);

TypeId 
MplsIpv4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsIpv4Protocol")
      .SetParent<Ipv4L3Protocol> ()
      .AddConstructor<MplsIpv4Protocol> ()
    ;
  return tid;
}

MplsIpv4Protocol::MplsIpv4Protocol()
  : m_routingProtocol (0)
{
  NS_LOG_FUNCTION (this);
}

MplsIpv4Protocol::~MplsIpv4Protocol ()
{
  NS_LOG_FUNCTION (this);
  m_routingProtocol = 0;
}

void
MplsIpv4Protocol::NotifyNewAggregate ()
{
  if (!m_routingProtocol)
    {
      // install MplsIpv4Routing if mpls protocol was installed
      Ptr<MplsProtocol> mpls = GetObject<MplsProtocol> ();
      if (mpls != 0)
        {
          m_routingProtocol = CreateObject<MplsIpv4Routing> ();
          m_routingProtocol->SetMpls (mpls);
          Ipv4L3Protocol::SetRoutingProtocol (m_routingProtocol);
        }
    }
    
  Ipv4L3Protocol::NotifyNewAggregate ();
}

void 
MplsIpv4Protocol::SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol)
{
  NS_ASSERT_MSG (m_routingProtocol, "Mpls protocol should be installed first");
  m_routingProtocol->SetRoutingProtocol (routingProtocol);
}

Ptr<Ipv4RoutingProtocol> 
MplsIpv4Protocol::GetRoutingProtocol (void) const
{
  NS_ASSERT_MSG (m_routingProtocol, "Mpls protocol should be installed first");
  return m_routingProtocol->GetRoutingProtocol ();
}

} // namespace mpls
} // namespace ns3
 