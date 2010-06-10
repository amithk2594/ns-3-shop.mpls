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

#ifndef MPLS_FORWARDING_TABLE_H
#define MPLS_FORWARDING_TABLE_H

#include <ostream>
#include <list>
#include <stdint.h>

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief MplsInterfaceForwardingTable represent per interface FIB
 */
class MplsInterfaceForwardingTable
{
public:
  /**
   * Create empty forwarding table
   */
  MplsInterfaceForwardingTable (const Ptr<MplsInterface> &interface);
  virtual ~MplsInterfaceForwardingTable ();
  /**
   * \brief Add ilm
   * \param label
   * \param nhlfe
   */
  void AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Get ilm
   * \param label
   * \returns ilm
   */
  Ptr<MplsIlm> GetIlm (const MplsLabel &label) const;
  /**
   * \brief Add fec
   * \param fec
   * \param nhlfe
   */
  void AddFec (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove NHLFE
   */
  void RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove NHLFE
   */
  void RemoveNhlfe (uint16_t lspid);

private:
  typedef std::list<Ptr<MplsFtn> > FtnTable;
  typedef std::list<Ptr<MplsIlm> > IlmTable;

  Ptr<MplsInterface> m_interface;
  FtnTable m_ftnTable;
  IlmTable m_ilmTable;
}

/**
 * \brief Mpls interface
 */
class MplsForwardingTable : public SimpleRefCount<MplsForwardingTable>
{
public:
  /**
   * Create empty FIB
   */
  MplsForwardingTable ();
  virtual ~MplsForwardingTable ();
  /**
   * Add ilm entry
   */
  void AddIlm (const Ptr<MplsInterface> &interface, const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * Add ftn entry
   */
  void AddFec (const Ptr<MplsInterface> &interface, const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe);

private:
  typedef std::list<Ptr<MplsIlm> > m_ilmTable;
  typedef std::list<Ptr<MplsFtn> > m_ftnTable;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
