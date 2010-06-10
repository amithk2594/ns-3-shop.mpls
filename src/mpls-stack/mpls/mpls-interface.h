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

namespace ns3 {
namespace mpls {

/**
 * \brief Mpls interface
 */
class MplsInterface : public SimpleRefCount<MplsInterface>
{
public:
  /**
   * \brief Create mpls interface
   */
  MplsInterface (const Ptr<NetDevice> &device);
  virtual ~MplsInterface ();
  /**
   * \brief Add label
   */
  void AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Add FEC
   */
  void AddFec (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove Ilm
   * \param label
   */
  void RemoveIlm (const MplsLabel &label);
  /**
   *
   */

  /**
   * \brief print interface information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;

private:
  typedef std::list<Ptr<MplsInterfaceForwardingTable> > ForwardingTable;
  ForwardingTable m_fTable;
  Ptr<NetDevice> m_device;
};

std::ostream& operator<< (std::ostream& os, const MplsInterface &interface);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
