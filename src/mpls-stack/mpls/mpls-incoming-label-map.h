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

#ifndef MPLS_INCOMING_LABEL_MAP_H
#define MPLS_INCOMING_LABEL_MAP_H

#include <ostream>
#include <stdint.h>

#include "mpls-generic.h"
#include "mpls-interface.h"
#include "mpls-forwarding-information.h"
#include "mpls-forwarding-information-base.h"

namespace ns3 {
namespace mpls {

class ForwardingInformationBase;
class Interface;

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
   * \brief Construct ILM with specified label and interface
   * \param interface incoming interface
   * \param label incoming label
   */
  IncomingLabelMap (const Ptr<Interface> &interface, Label label);
  /**
   * \brief Destuctor
   */
  virtual ~IncomingLabelMap ();
  /**
   * \brief Get incoming label
   * \returns label
   */
  Label GetLabel (void) const;
  /**
   * \brief Get incoming interface
   * \returns interface
   */
  const Ptr<Interface>& GetInterface (void) const;
  /**
   * \brief Print ILM
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  IncomingLabelMap ();

private:
  Ptr<Interface>  m_interface;
  Label           m_label;

  friend class ForwardingInformationBase;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INCOMING_LABEL_MAP_H */
