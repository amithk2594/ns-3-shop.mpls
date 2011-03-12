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
  m_ifup = true;
}

void
Interface::SetDown ()
{
  m_ifup = false;
}

void
Interface::Send (Ptr<Packet>& packet)
{
// /* restore IpHeader */
//  if (ipHeader != 0)
//    {
//      packet->AddHeader (*ipHeader);
//    }

//  /* place MPLS shim */
//  packet->AddHeader (stack);

//  if (packet->GetSize () > outDev->GetMtu ())
//    {
//      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::MplsForward (): "
//                    "dropping received packet -- MTU size exceeded");
//      // XXX: need MTU Path Discover algoritm
//      return;
//    }

//  // XXX: now only PointToPoint devices supported
//  outDev->Send (packet, outDev->GetBroadcast (), PROT_NUMBER);
}

} // namespace mpls
} // namespace ns3
