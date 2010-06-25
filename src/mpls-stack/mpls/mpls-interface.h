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

#ifndef MPLS_INTERFACE_H
#define MPLS_INTERFACE_H

#include <ostream>
#include <list>
#include <stdint.h>

#include "ns3/object.h"
#include "ns3/net-device.h"

#include "mpls-generic.h"
#include "mpls-fib.h"
#include "mpls-nhlfe.h"
#include "mpls-ilm.h"
#include "mpls-fec.h"

namespace ns3 {
namespace mpls {

class MplsFib;

/**
 * \ingroup mpls
 * \brief
 * Mpls interface
 */
class MplsInterface : public Object
{
public:
  static TypeId GetTypeId (void);
  /**
   * \brief Create mpls interface
   */
  MplsInterface ();
  /**
   * \brief Destructor
   */
  virtual ~MplsInterface ();

  void SetDevice (const Ptr<NetDevice> &device);
  void SetNode (const Ptr<Node> &node);

  /**
   * \brief Set strict mode
   * \param strict
   */
  void SetStrict (bool strict);
  /**
   * \brief Get device associated with interface
   */
  Ptr<NetDevice> GetDevice (void) const;
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
  Ptr<MplsIlm> GetIlm (const MplsLabel &label) const;
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
   * \brief Remove NHLFE
   * \param nhlfe NHLFE to remove
   */
  void RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Get device associated with interface
   */
  const Ptr<MplsFib>& GetForwardingTable (void) const;
  /**
   * \brief Print interface information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

protected:
  virtual void DoDispose (void);

private:
  void DoSetup (void);

  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
  Ptr<MplsFib> m_fib;
  bool m_strict;
};

std::ostream& operator<< (std::ostream& os, const Ptr<MplsInterface> &interface);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
