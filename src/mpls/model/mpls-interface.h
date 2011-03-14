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

#ifndef MPLS_INTERFACE_H
#define MPLS_INTERFACE_H

#include <ostream>
#include <stdint.h>

#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/packet.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief
 * Mpls interface
 */
class Interface : public Object
{
public:
  static TypeId GetTypeId (void);
  /**
   * @brief Create mpls interface
   */
  Interface (int32_t ifIndex);
  /**
   * @brief Destructor
   */
  virtual ~Interface ();
  /**
   * @brief Returns true if this interface is enabled  
   */
  bool IsUp () const;
  /**
   * @brief Returns true if this interface is disabled
   */
  bool IsDown () const;
  /**
   * @brief Enable interface
   */
  void SetUp ();
  /**
   * @brief Disable interface
   */
  void SetDown ();
  /**
   * @brief Set device
   */
  void SetDevice (const Ptr<NetDevice> &device);
  /**
   * @brief Set node
   */  
  void SetNode (const Ptr<Node> &node);
  /**
   * @brier Send packet 
   */
  bool Send (const Ptr<Packet>& packet);
  /**
   * @brief Get device associated with interface
   */
  Ptr<NetDevice>& GetDevice (void);
  /**
   * @brief Returns interface index
   */
  int32_t GetIfIndex (void);  

protected:
  virtual void DoDispose (void);

private:
  void DoSetup (void);

  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
  bool m_ifup;
  int32_t m_ifIndex;  
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
