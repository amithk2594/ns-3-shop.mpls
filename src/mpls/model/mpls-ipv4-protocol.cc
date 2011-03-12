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

#include "mpls-ipv4-protocol.h"

NS_LOG_COMPONENT_DEFINE ("mpls::Ipv4Protocol");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (Ipv4Protocol);

TypeId
Ipv4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Ipv4Protocol")
      .SetParent<Ipv4L3Protocol> ()
      .AddConstructor<Ipv4Protocol> ()
    ;
  return tid;
}

Ipv4Protocol::Ipv4Protocol()
  : m_routingProtocol (0),
    m_mpls (0)
{
  NS_LOG_FUNCTION (this);
}

Ipv4Protocol::~Ipv4Protocol ()
{
  NS_LOG_FUNCTION (this);
  m_routingProtocol = 0;
  m_mpls = 0;
}

void
Ipv4Protocol::NotifyNewAggregate ()
{
  if (!m_mpls)
    {
      Ptr<Mpls> mpls = GetObject<Mpls> ();
      if (mpls != 0)
        {
          m_mpls = mpls;
          m_routingProtocol = CreateObject<mpls::Ipv4Routing> ();
          m_routingProtocol->SetIpv4 (this);
          m_routingProtocol->SetMpls (mpls);
          Ipv4L3Protocol::SetRoutingProtocol (m_routingProtocol);
        }
    }

  Ipv4L3Protocol::NotifyNewAggregate ();
}

uint32_t
Ipv4Protocol::AddInterface (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << &device);

  NS_ASSERT_MSG (m_mpls != 0, "Mpls protocol should be installed first");

  uint32_t index = Ipv4L3Protocol::AddInterface (device);

  if (m_mpls->IsNewInterfaceNotificationEnabled ())
    {
      m_mpls->NotifyNewInterface (device);
    }

  return index;
}

void
Ipv4Protocol::SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol)
{
  NS_LOG_FUNCTION (this << routingProtocol);
  
  NS_ASSERT_MSG (m_routingProtocol != 0, "Mpls protocol should be installed first");
  m_routingProtocol->SetRoutingProtocol (routingProtocol);
}

Ptr<Ipv4RoutingProtocol>
Ipv4Protocol::GetRoutingProtocol (void) const
{
  NS_ASSERT_MSG (m_routingProtocol != 0, "Mpls protocol should be installed first");
  return m_routingProtocol->GetRoutingProtocol ();
}

} // namespace mpls
} // namespace ns3

