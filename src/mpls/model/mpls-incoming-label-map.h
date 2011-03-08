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
 
#ifndef MPLS_INCOMING_LABEL_MAP_H
#define MPLS_INCOMING_LABEL_MAP_H

#include <ostream>

#include "mpls-label.h"
#include "mpls-forwarding-information.h"

namespace ns3 {
namespace mpls {

class ForwardingInformation;

/**
 * \ingroup mpls
 * \brief
 * The "Incoming Label Map" (ILM) maps each incoming label to a set of NHLFEs.
 * It is used when forwarding packets that arrive as labeled packets.
 * For more infomation see RFC 3031 (http://www.ietf.org/rfc/rfc3031.txt)
 */
class IncomingLabelMap : public ForwardingInformation
{
public:
  /**
   * @brief Construct ILM for specified incoming interface and label
   * @param interface incoming interface
   * @param label incoming label
   * @param nhlfe NHLFE (at least one NHLFE should be set)
   */
  IncomingLabelMap (uint32_t interface, Label label, const Nhlfe &nhlfe);
  /**
   * @brief Construct ILM for specified incoming label
   * @param label incoming label
   * @param nhlfe NHLFE (at least one NHLFE should be set)
   */
  IncomingLabelMap (Label label, const Nhlfe &nhlfe);
  /**
   * @brief Destuctor
   */
  virtual ~IncomingLabelMap ();
  /**
   * @brief Get incoming label
   */
  Label GetLabel (void) const;
  /**
   * @brief Get incoming interface
   */
  uint32_t GetInterface (void) const;
  /**
   * @brief Set incoming label
   */
  void SetLabel (Label label);
  /**
   * @brief Set incoming interface
   */
  void SetInterface (uint32_t interface);
  /**
   * @brief Print ILM
   * @param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  IncomingLabelMap ();

private:
  uint32_t m_interface;
  Label m_label;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INCOMING_LABEL_MAP_H */
