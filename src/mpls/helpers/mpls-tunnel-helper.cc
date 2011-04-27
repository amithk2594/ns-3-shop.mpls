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
  : m_tunnels (Create<TunnelMap> ())
{
}

MplsTunnelHelper::~MplsTunnelHelper(void)
{
  m_tunnels = 0;
}

MplsTunnelHelper::MplsTunnelHelper (const MplsTunnelHelper &o)
{
  m_tunnels = o.m_tunnels;
}

MplsTunnelHelper& 
MplsTunnelHelper::operator= (const MplsTunnelHelper &o)
{
  m_tunnels = o.m_tunnels;
  return *this;
}

TunnelId 
MplsTunnelHelper::CreateTunnel (const TunnelNodeList &nodes)
{
  NS_ASSERT_MSG (nodes.GetN () >= 2, "Invalid number of nodes");

//  TunnelNodeList::Iterator i = nodes.Begin ();
//    
//  std::set<Ptr<Node> > nodes;
//  nodes.insert (*i);
//  
//  Ptr<Interface> mplsIf = FindInterfaceForAddress (0, (*i).GetAddress ());

//  ++i;
//  
//  for (TunnelNodeList::Iterator j = nodes.End (); i != j; ++i)
//    {
//      Ptr<Interface> mplsIf = FindInterfaceForAddress (mplsIf, (*i).GetAddress ());
//      std::cout << mplsIf << std::endl;
//    }

  return TunnelId (0);
}

void
MplsTunnelHelper::DestroyTunnel (const TunnelId &tunnel)
{
}

Ptr<Interface>
MplsTunnelHelper::FindInterfaceForAddress (const Ipv4Address &addr)
{
  const NodeContainer& nodes = GetNetworkNodes ();
  
  for (NodeContainer::Iterator node = nodes.Begin (), end = nodes.End (); node != end; ++node)
    {
      Ptr<Mpls> mpls = (*node)->GetObject<Mpls> ();
      
      if (mpls == 0)
        {
          continue;
        }
      
      for (int32_t i = 0, n = mpls->GetNInterfaces (); i < n; ++i)
        {
          Ptr<Interface> mplsIf = mpls->GetInterface (i);
          Ptr<Ipv4Interface> ipv4If = mplsIf->GetObject<Ipv4Interface> ();
          if (ipv4If == 0) 
            {
              continue;
            }

          for (int32_t j = 0, m = ipv4If->GetNAddresses (); j < m; ++j)
            {
              if (addr.IsEqual(ipv4If->GetAddress (j).GetLocal ()))
                {
                  return mplsIf;
                }
            }
        }
    }

  return 0;
}

}// namespace ns3
