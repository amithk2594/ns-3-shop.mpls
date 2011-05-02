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

#ifndef MPLS_NODE_H
#define MPLS_NODE_H

#include <ostream>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "mpls-incoming-label-map.h"
#include "mpls-fec-to-nhlfe.h"
#include "mpls-label-space.h"
#include "mpls-label.h"

namespace ns3 {

namespace mpls {
class IncomingLabelMap;
class FecToNhlfe;
}

using namespace mpls;

/**
 * \ingroup mpls
 */
class MplsNode : public Node
{
public:
  typedef std::list<Ptr<IncomingLabelMap> > IlmTable;
  typedef std::list<Ptr<FecToNhlfe> > FtnTable;
  
  enum LabelSpaceType {
    PLATFORM = 0,
    INTERFACE
  };
  
  static TypeId GetTypeId (void);

  MplsNode ();
  virtual ~MplsNode ();

  /**
   * @brief Get ILM table
   */
  IlmTable* GetIlmTable (void);
  /**
   * @brief Get Ftn table
   */
  FtnTable* GetFtnTable (void);
  /**
   * @brief Lookup ilm
   */
  Ptr<IncomingLabelMap> LookupIlm (Label label, int32_t interface);
  /**
   * @brief Lookup ftn
   */
  Ptr<FecToNhlfe> LookupFtn (PacketDemux& demux);
  /**
   * @brief Returns label space for the specified interface
   */
  LabelSpace* GetLabelSpace (uint32_t ifIndex);
  /**
   * @brief Set label space type
   */
  void SetLabelSpaceType (LabelSpaceType type);
  /**
   * @brief Set minimum label value
   */
  void SetMinLabelValue (uint32_t value);
  /**
   * @brief Set maximum label value
   */
  void SetMaxLabelValue (uint32_t value);
  
private:
  
  IlmTable m_ilmTable;
  FtnTable m_ftnTable;
  LabelSpaceType m_labelSpaceType;
  LabelSpace m_labelSpace;
  bool m_interfaceAutoInstall;
};

} // namespace ns3

#endif /* MPLS_NODE_H */
