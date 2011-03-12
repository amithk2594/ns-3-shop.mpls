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

#include "mpls-label.h"
#include "mpls-interface.h"
#include "mpls-label-stack.h"
#include "mpls-forwarding-information.h"
#include "mpls-incoming-label-map.h"
#include "mpls-fec-to-nhlfe.h"
#include "mpls-nhlfe.h"
#include "mpls-ilm-table.h"
#include "mpls-ftn-table.h"

namespace ns3 {

using namespace mpls;

/**
 * \ingroup mpls
 * Multiprotocol Label Switching. RFC 3031, 3032.
 */
class Mpls : public Object
{
public:
  static TypeId GetTypeId (void);

  Mpls ();
  virtual ~Mpls ();

  /**
   * @brief Enable/disable new interface notify (default disabled)
   */
  void EnableNewInterfaceNotification (bool enabled);
  bool IsNewInterfaceNotificationEnabled (void) const;
  virtual void NotifyNewInterface (const Ptr<NetDevice> &device) = 0;

  /**
   * @brief Set new ILM table
   */
  virtual void SetIlmTable (const Ptr<IlmTable> &ilmTable) = 0;
  /**
   * @brief Get ILM table
   */
  virtual Ptr<IlmTable> GetIlmTable (void) const = 0;
  /**
   * @brief Set new FTN table
   */
  virtual void SetFtnTable (const Ptr<FtnTable> &ftnTable) = 0;
  /**
   * @brief Get Ftn table
   */
  virtual Ptr<FtnTable> GetFtnTable (void) const = 0;
  /**
   * @param device device to add to the list of Mpls interfaces
   * @return interface index of the Mpls interface added
   */
  virtual int32_t AddInterface (const Ptr<NetDevice> &device) = 0;
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
   * @brief Forward packet according to forwarding information
   */
  virtual void MplsForward (Ptr<Packet> &packet, const Ptr<ForwardingInformation> &fwd, LabelStack &stack, int8_t ttl) = 0;
  /**
   * @brief Lookup ILM by label and interface
   */
  virtual Ptr<IncomingLabelMap> LookupIlm (Label label, int32_t interface) = 0;
  /**
   * @brief Lookup FTN using PacketDemux
   */
  virtual Ptr<FecToNhlfe> LookupFtn (PacketDemux& demux) = 0;

private:
  bool m_newInterfaceNotification;
};

} // namespace ns3

#endif /* MPLS_H */
