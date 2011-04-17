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

#include <iostream>

#include "ns3/assert.h"
#include "ns3/log.h"
#include "mpls-tunnel-node.h"

namespace ns3 {

using namespace mpls;

TunnelNode::TunnelNode (const Ipv4Address &address)
  : m_address (address)
{
}

TunnelNode::TunnelNode (const char *address)
  : m_address (Ipv4Address (address))
{
}

TunnelNode::~TunnelNode ()
{
}

TunnelNode::TunnelNode (const TunnelNode &o)
{
  m_address = o.m_address;
}

TunnelNode&
TunnelNode::operator= (const TunnelNode &o)
{
  if (this == &o)
    {
      return *this;
    }

  m_address = o.m_address;
  return *this;
}

TunnelNodeList
TunnelNode::operator>> (const TunnelNode &o)
{
  TunnelNodeList tunnel;
  tunnel.Add (*this);
  tunnel.Add (o);  
  return tunnel;
}

const Ipv4Address&
TunnelNode::GetAddress (void) const
{
  return m_address;
}

TunnelNodeList::TunnelNodeList()
{
}

TunnelNodeList::~TunnelNodeList()
{
}

TunnelNodeList::TunnelNodeList (const TunnelNodeList &o)
{
  m_nodes = o.m_nodes;
}

TunnelNodeList&
TunnelNodeList::operator= (const TunnelNodeList &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_nodes = o.m_nodes;
  return *this;
}

  
TunnelNodeList&
TunnelNodeList::Add (const TunnelNode &o)
{
  m_nodes.push_back (o);
  return *this;  
}

TunnelNodeList&
TunnelNodeList::Add (const TunnelNodeList &o)
{
  m_nodes.insert (m_nodes.end (), o.m_nodes.begin (), o.m_nodes.end ());
  return *this;
}

  
TunnelNodeList
TunnelNodeList::operator>> (const TunnelNodeList &o)
{
  TunnelNodeList tunnel (*this);
  tunnel.Add (o);
  return tunnel;
}

TunnelNodeList
TunnelNodeList::operator>> (const TunnelNode &o)
{
  TunnelNodeList tunnel (*this);
  tunnel.Add (o);
  return tunnel;
}

TunnelNodeList::Iterator
TunnelNodeList::Begin (void) const
{
  return m_nodes.begin ();
}

TunnelNodeList::Iterator
TunnelNodeList::End (void) const
{
  return m_nodes.end ();
}

uint32_t
TunnelNodeList::GetN (void) const
{
  return m_nodes.size ();
}

std::ostream& 
operator<< (std::ostream& os, const TunnelNode &node)
{
  os << node.m_address;
  return os;
}

std::ostream& 
operator<< (std::ostream& os, const TunnelNodeList &nodes)
{
  for (TunnelNodeList::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      os << *i << " ";
    }
  return os;
}

}// namespace ns3
