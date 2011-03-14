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

#define NS_LOG_APPEND_CONTEXT \
    if (m_node != 0) { \
      std::clog << Simulator::Now ().GetSeconds () \
      << " [node " << m_node->GetId () << "] "; }

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

#include "mpls.h"
#include "mpls-interface.h"

NS_LOG_COMPONENT_DEFINE ("mpls::Interface");

namespace ns3 {
namespace mpls {

TypeId
Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Interface")
    .SetParent<Object> ()
    ;
  return tid;
}

Interface::Interface (int32_t ifIndex)
  : m_node (0),
    m_device (0),
    m_ifup (true),
    m_ifIndex (ifIndex)
{
  NS_LOG_FUNCTION (this);
}

Interface::~Interface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Interface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void
Interface::SetNode (const Ptr<Node> &node)
{
  m_node = node;
}

void
Interface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
}

Ptr<NetDevice>&
Interface::GetDevice (void)
{
  return m_device;
}

int32_t
Interface::GetIfIndex (void)
{
  return m_ifIndex;
}
  
bool
Interface::IsUp () const
{
  return m_ifup;
}

bool
Interface::IsDown () const
{
  return !m_ifup;
}

void
Interface::SetUp ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = true;
}

void
Interface::SetDown ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = false;
}

bool
Interface::Send (const Ptr<Packet>& packet)
{
  NS_LOG_FUNCTION (this << packet);

  if (packet->GetSize () > m_device->GetMtu ())
    {
      NS_LOG_LOGIC ("dropping received packet -- MTU size exceeded");
      return false;
    }

  // XXX: now only PointToPoint devices supported
  m_device->Send (packet, m_device->GetBroadcast (), Mpls::PROT_NUMBER);
  return true;
}

} // namespace mpls
} // namespace ns3
