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

#ifndef MPLS_H
#define MPLS_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/packet.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv6-interface.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"

#include "mpls-interface.h"

namespace ns3 {

using namespace mpls;

/**
 * \defgroup mpls
 */

/**
 * \ingroup mpls
 * Multiprotocol Label Switching. RFC 3031, 3032.
 */
class Mpls : public Object
{
public:
  static TypeId GetTypeId (void);

  static const uint16_t PROT_NUMBER;

  Mpls ();
  virtual ~Mpls ();

  /**
   * @brief Enable/disable new interface notify (default disabled)
   */
  void EnableNewInterfaceNotification (bool enabled);
  bool IsNewInterfaceNotificationEnabled (void) const;
  
  virtual void NotifyNewInterface (const Ptr<Ipv4Interface> &iface) = 0;
  virtual void NotifyNewInterface (const Ptr<Ipv6Interface> &iface) = 0;

  /**
   * @param device device to add to the list of Mpls interfaces
   * @return the Mpls interface
   */
  virtual Ptr<Interface> AddInterface (const Ptr<NetDevice> &device) = 0;
  /**
   * @brief Get Mpls interface by index
   * @param index Mpls interface index
   */
  virtual Ptr<Interface> GetInterface (int32_t index) const = 0;
  /**
   * @brief Get Mpls interface for specified device
   * @return Mpls interface
   */
  virtual Ptr<Interface> GetInterfaceForDevice (const Ptr<const NetDevice> &device) const = 0;
  /**
   * @brief The number of Mpls interfaces added
   */
  virtual uint32_t GetNInterfaces (void) const = 0;
  /**
   * @brief ipv4 unlabeled packet entry point
   */
  virtual bool ReceiveIpv4 (const Ptr<Packet> &packet, const Ipv4Header &header, 
                              const Ptr<const NetDevice> &device) = 0;
  /**
   * @brief ipv6 unlabeled packet entry point
   */
  virtual bool ReceiveIpv6 (const Ptr<Packet> &packet, const Ipv6Header &header, 
                              const Ptr<const NetDevice> &device) = 0;  

private:
  bool m_newInterfaceNotification;
};

} // namespace ns3

#endif /* MPLS_H */
