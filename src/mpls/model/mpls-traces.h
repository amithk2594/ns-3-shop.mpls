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

#ifndef MPLS_TRACES_H
#define MPLS_TRACES_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/traced-callback.h"

#include "mpls-interface.h"

namespace ns3 {
namespace mpls {
namespace traces {

/**
 * \ingroup mpls
 * @enum DropReason
 * @brief Reason why a packet has been dropped.
 */
enum DropReason
  {
    DROP_TTL_EXPIRED = 1,            /**< Packet TTL has expired */
    DROP_MTU_EXCEEDED,               /**< Packet size is greater than the device MTU */
    DROP_FTN_NOT_FOUND,              /**< An FTN matching the packet has not been found */
    DROP_ILM_NOT_FOUND,              /**< An ILM matching the label has not been found */
    DROP_NO_SUITABLE_NHLFE,          /**< An NHLFE suitable to process the packet has not been found */
    DROP_EMPTY_STACK,                /**< Empty label stack */
    DROP_BROADCAST_NOT_SUPPORTED,    /**< Received a broadcast packet */
    DROP_MULTICAST_NOT_SUPPORTED,    /**< Received a multicast packet */
    DROP_IPV6_NOT_SUPPORTED,         /**< Received an IPv6 packet */
    DROP_ILLEGAL_IPV4_EXPLICIT_NULL, /**< IPv4 Explicit Null label not at the bottom of the stack */
    DROP_ILLEGAL_IPV6_EXPLICIT_NULL, /**< IPv6 Explicit Null label not at the bottom of the stack */
    DROP_NO_IPV4,                    /**< IPv4 is not installed on the node */
    DROP_INTERFACE_DOWN,             /**< Interface is down so can not send packet */
    DROP_SEND_FAILED,                /**< Sending the packet through the identified interface failed */
  };


typedef TracedCallback<const Ptr<const Packet>&, int32_t > RxTracedCallback;
typedef TracedCallback<const Ptr<const Packet>&, int32_t > TxTracedCallback;
typedef TracedCallback<const Ptr<const Packet>&, DropReason, int32_t > DropTracedCallback;

} // namespace traces
} // namespace mpls
} // namespace ns3

#endif /* MPLS_TRACES_H */
