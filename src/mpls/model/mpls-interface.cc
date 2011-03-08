/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
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
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "mpls-interface.h"

NS_LOG_COMPONENT_DEFINE ("MplsInterface");

namespace ns3 {
namespace mpls {

TypeId
MplsInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsInterface")
    .SetParent<Object> ()
    ;
  return tid;
}

MplsInterface::MplsInterface ()
  : m_node (0),
    m_device (0),
    m_ifup (true)
{
  NS_LOG_FUNCTION (this);
}

MplsInterface::~MplsInterface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MplsInterface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void
MplsInterface::SetNode (const Ptr<Node> &node)
{
  m_node = node;
}

void
MplsInterface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
}

Ptr<NetDevice>&
MplsInterface::GetDevice (void)
{
  return m_device;
}

bool
MplsInterface::IsUp () const
{
  return m_ifup;
}

bool
MplsInterface::IsDown () const
{
  return !m_ifup;
}

void
MplsInterface::SetUp ()
{
  m_ifup = true;
}

void
MplsInterface::SetDown ()
{
  m_ifup = false;
}

void
MplsInterface::Send (Ptr<Packet>& packet)
{
}
  
void
MplsInterface::Print (std::ostream &os) const
{
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsInterface> &interface)
{
  interface->Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
