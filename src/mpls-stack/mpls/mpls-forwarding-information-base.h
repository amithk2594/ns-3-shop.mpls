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

#ifndef MPLS_FORWARDING_INFORMATION_BASE_H
#define MPLS_FORWARDING_INFORMATION_BASE_H

#include <ostream>
#include <list>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-generic.h"
#include "mpls-incoming-label-map.h"
#include "mpls-next-hop-label-forwarding-entry.h"
#include "mpls-fec-to-nhlfe.h"
#include "mpls-forwarding-equivalence-class.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief ForwardingInformationBase represent per interface FIB
 */
class ForwardingInformationBase: public SimpleRefCount<ForwardingInformationBase>
{
public:
  /**
   * \brief Create empty forwarding base
   */
  ForwardingInformationBase ();
  /**
   * \brief Destructor
   */
  virtual ~ForwardingInformationBase ();
  /**
   * \brief Add ILM
   * \param label
   * \param nhlfe
   */
  void AddIlm (Label label, const Ptr<NextHopLabelForwardingEntry> &nhlfe);
  /**
   * \brief Get ILM
   * \param label
   * \returns ILM associated with label
   */
  Ptr<IncomingLabelMap>& GetIlm (Label label) const;
  /**
   * \brief Remove ILM
   * \param label
   */
  void RemoveIlm (Label label);
  /**
   * \brief Remove ILM
   * \param ilm
   */
  void RemoveIlm (const Ptr<IncomingLabelMap> &ilm);
  /**
   * \brief Add FTN
   * \param fec
   * \param nhlfe
   */
  void AddFtn (const Ptr<ForwardingEquivalenceClass> &fec, const Ptr<NextHopLabelForwardingEntry> &nhlfe);
  /**
   * \brief Get FTN
   * \param fec
   * \returns FTN associated with FEC
   */
  Ptr<FecToNhlfe> GetFtn (const Ptr<ForwardingEquivalenceClass> &fec) const;
  /**
   * \brief Remove FTN
   * \param fec
   */
  void RemoveFtn (const Ptr<ForwardingEquivalenceClass> &fec);
  /**
   * \brief Remove FTN
   * \param ftn FTN to remove
   */
  void RemoveFtn (const Ptr<FecToNhlfe> &ftn);
  /**
   * \brief Get NHLFE for specified Ipv4 packet
   * \param packet Ipv4 packet
   * \param header Ipv4 header
   * \returns NHLFE
   */
  Ptr<NextHopLabelForwardingEntry> GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const;
  /**
   * \brief Get NHLFE for specified Ipv4 packet
   * \param packet Ipv6 packet
   * \param header Ipv6 header
   * \returns NHLFE
   */
  Ptr<NextHopLabelForwardingEntry> GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const;
  /**
   * \brief Remove NHLFE
   * \param nhlfe NHLFE to remove
   */
  void RemoveNhlfe (const Ptr<NextHopLabelForwardingEntry> &nhlfe);
  /**
   * \brief Print forwarding information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  typedef std::list<Ptr<FecToNhlfe> > FtnList;
  typedef std::list<Ptr<IncomingLabelMap> > IlmList;

  FtnList m_ftnTable;
  IlmList m_ilmTable;
}

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformationBase> &fib);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_BASE_H */
