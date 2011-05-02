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

#include "mpls-tunnel-helper.h"

namespace ns3 {

using namespace mpls;

TunnelId::TunnelId (uint32_t id)
  : m_id (id)
{
}
  
TunnelId::~TunnelId(void)
{
}

size_t
TunnelId::Hash::operator() (TunnelId const &x) const
{
  return x.m_id;
}


LspNode::LspNode (const char* addr)
  : m_address (Ipv4Address(addr)),
    m_node (0)
{
}


LspNode::LspNode (const Ipv4Address &addr)
  : m_address (addr),
    m_node (0)
{
}


LspNode::LspNode (const Ptr<Node> &node)
  : m_address (),
    m_node (node)
{
}

LspNode::~LspNode ()
{
  m_node = 0;
}
    
Lsp
LspNode::operator+ (const LspNode &x) const
{
  return Lsp().Add (*this).Add (x);
}

const Ipv4Address& 
LspNode::GetAddress (void) const
{
  return m_address;
}

const Ptr<Node>& 
LspNode::GetNode (void) const
{
  return m_node;
}


Lsp::Lsp ()
{
}

Lsp::Lsp (const Lsp &lsp)
{
  m_nodes = lsp.m_nodes;
}

Lsp::~Lsp ()
{
}

Lsp&
Lsp::Add (const LspNode &node)
{
  m_nodes.push_back (node);
  return *this;
}

Lsp&
Lsp::operator= (const Lsp &lsp)
{
  if (this == &lsp)
    {
      return *this;
    }
  m_nodes = lsp.m_nodes;
  return *this;
}

Lsp
Lsp::operator+ (const LspNode &node) const
{
  Lsp tmp (*this);
  return tmp.Add (node);
}

Lsp
Lsp::operator+ (const Lsp &lsp) const
{
  Lsp tmp (*this);
  tmp.m_nodes.insert (tmp.m_nodes.end(), lsp.Begin (), lsp.End ());
  return tmp;
}
  
Lsp::Iterator
Lsp::Begin (void) const
{
  return m_nodes.begin ();
}

Lsp::Iterator
Lsp::End (void) const
{
  return m_nodes.end ();
}

MplsTunnelHelper::MplsTunnelHelper(void)
{
}

MplsTunnelHelper::~MplsTunnelHelper(void)
{
}

MplsTunnelHelper::MplsTunnelHelper (const MplsTunnelHelper &o)
{
  //m_tunnels = o.m_tunnels;
}

MplsTunnelHelper& 
MplsTunnelHelper::operator= (const MplsTunnelHelper &o)
{
  //m_tunnels = o.m_tunnels;
  return *this;
}

TunnelId 
MplsTunnelHelper::CreateTunnel (const Lsp &lsp)
{
  return TunnelId (0);
}

void
MplsTunnelHelper::DestroyTunnel (const TunnelId &tunnel)
{
}

}// namespace ns3
