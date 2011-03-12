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

#include "ns3/names.h"

#include "mpls-node-helper.h"

namespace ns3 {

ForEachNode (std::string nodeName, const ForEachNodeCallback& cb)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  cb (node);
}

ForEachNode (Ptr<Node> node, const ForEachNodeCallback& cb)
{
  cb (node);
}

ForEachNode (const NodeContainer& c, const ForEachNodeCallback& cb)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      cb (*i);
    }
}

} // namespace ns3
