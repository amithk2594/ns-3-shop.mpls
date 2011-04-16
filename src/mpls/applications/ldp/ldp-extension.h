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

#ifndef LDP_EXTENSION_H
#define LDP_EXTENSION_H

#include "ns3/object.h"
#include "ns3/ptr.h"

#include "protocol-data-unit.h"
#include "ldp-peer.h"

namespace ns3 {
namespace ldp {

class LdpPeer;

class LdpExtension : public Object
{
public:
  static TypeId GetTypeId (void);

  LdpExtension ();
  virtual ~LdpExtension();

  virtual bool ReceiveMessage (Ptr<LdpPeer> peer, Ptr<const Message> message, uint32_t &errno) = 0;

};

} // namespace ldp
} // namespace ns3

#endif /* LDP_EXTENSION_H */
