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
#include "ns3/object.h"
#include "ns3/names.h"

#include "mpls-node-helper.h"

namespace ns3 {

_MplsNodeHelper::_MplsNodeHelper ()
  : m_node (0)
{
}

_MplsNodeHelper::_MplsNodeHelper (const Ptr<Node> &node)
{
  SetNode (node);
}

_MplsNodeHelper::_MplsNodeHelper (const std::string &node)
{
  SetNode (node);
}

_MplsNodeHelper::~_MplsNodeHelper()
{
  m_node = 0;
}

void
_MplsNodeHelper::SetNode (const Ptr<Node> &node)
{
  NS_ASSERT (node != 0);

  NS_ASSERT_MSG (node->GetObject<Mpls> () != 0, "There is no mpls installed on specified node");
  
  m_node = node;
}

void
_MplsNodeHelper::SetNode (const std::string &nodeName)
{
  SetNode (Names::Find<Node> (nodeName));
}

Ptr<Mpls>
_MplsNodeHelper::GetMpls (void) const
{
  NS_ASSERT_MSG (m_node != 0, "Node is not specified");
  return m_node->GetObject<Mpls> ();
}

Ptr<Node>
_MplsNodeHelper::GetNode (void) const
{
  return m_node;
}

} // namespace ns3
