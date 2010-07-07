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

#ifndef MPLS_FIB_H
#define MPLS_FIB_H

#include <ostream>
#include <list>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-generic.h"
#include "mpls-interface.h"

namespace ns3 {
namespace mpls {

class MplsInterface;
/**
 * \ingroup mpls
 * \brief
 * MplsFib represent per interface FIB
 */
class MplsFib: public SimpleRefCount<MplsFib>
{
public:
  /**
   * \brief Create empty forwarding base
   */
  MplsFib ();
  /**
   * \brief Destructor
   */
  virtual ~MplsFib ();
  /**
   * \brief Add ILM
   * \param label
   * \param nhlfe
   */
  void AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Get ILM
   * \param label
   * \returns ILM associated with label
   */
  Ptr<MplsIlm>& GetIlm (const MplsLabel &label) const;
  /**
   * \brief Remove ILM
   * \param label
   */
  void RemoveIlm (const MplsLabel &label);
  /**
   * \brief Remove ILM
   * \param ilm
   */
  void RemoveIlm (const Ptr<MplsIlm> &ilm);
  /**
   * \brief Add FTN
   * \param fec
   * \param nhlfe
   */
  void AddFtn (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Get FTN
   * \param fec
   * \returns FTN associated with FEC
   */
  Ptr<MplsFtn> GetFtn (const Ptr<MplsFec> &fec) const;
  /**
   * \brief Remove FTN
   * \param fec
   */
  void RemoveFtn (const Ptr<MplsFec> &fec);
  /**
   * \brief Remove FTN
   * \param ftn FTN to remove
   */
  void RemoveFtn (const Ptr<MplsFtn> &ftn);
  /**
   * \brief Get NHLFE for specified Ipv4 packet
   * \param packet Ipv4 packet
   * \param header Ipv4 header
   * \returns NHLFE
   */
  Ptr<MplsNhlfe> GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const;
  /**
   * \brief Get NHLFE for specified Ipv4 packet
   * \param packet Ipv6 packet
   * \param header Ipv6 header
   * \returns NHLFE
   */
  Ptr<MplsNhlfe> GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const;
  /**
   * \brief Remove NHLFE
   * \param nhlfe NHLFE to remove
   */
  void RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Print forwarding information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  typedef std::list<Ptr<MplsFtn> > FtnList;
  typedef std::list<Ptr<MplsIlm> > IlmList;

  FtnList m_ftnTable;
  IlmList m_ilmTable;
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsFib> &fib);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FIB_H */
