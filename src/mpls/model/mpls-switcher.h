/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin, Stefano Avallone
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

#ifndef MPLS_SWITCHER_H
#define MPLS_SWITCHER_H

#include <ostream>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/callback.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief Mpls Swicther.
 *
 * This class holds together:
 *   - a list of NetDevice objects which represent the network interfaces
 *     of this node which are connected to other Node instances through
 *     Channel instances.
 *   - a list of Application objects which represent the userspace
 *     traffic generation applications which interact with the Node
 *     through the Socket API.
 *   - a node Id: a unique per-node identifier.
 */
class MplsSwitcher : public Node
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Constructor
   */
  MplsSwitcher ();
  /**
   * @brief Destructor
   */
  virtual ~MplsSwitcher ();

protected:
  virtual void NotifyNewAggregate ();
  virtual void DoDispose (void);
  virtual void DoStart (void);

};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_ROUTING_H */
