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

#include "mpls-enum-helper.h"

namespace ns3 {

void
ForEachNode (std::string nodeName, const ForEachNodeCallback& cb)
{
  Ptr<MplsNode> node = Names::Find<MplsNode> (nodeName);
  cb (node);
}

void
ForEachNode (Ptr<MplsNode> node, const ForEachNodeCallback& cb)
{
  cb (node);
}

void
ForEachNode (const NodeContainer& c, const ForEachNodeCallback& cb)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      cb (DynamicCast<MplsNode> (*i));
    }
}

} // namespace ns3
