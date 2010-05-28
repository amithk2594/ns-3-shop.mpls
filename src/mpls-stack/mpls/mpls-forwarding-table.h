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
 * \brief MplsForwardingTableEntry represent per interface FIB
 */
class MplsForwardingTableEntry
{
public:
  /**
   * Create empty forwarding table
   */
  MplsForwardingTableEntry (const Ptr<MplsInterface> &interface);
  virtual ~MplsForwardingTableEntry ();

  void AddIlm (const MplsLabel &label);
  void AddFec (const Ptr<MplsFec> &fec);

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
  void AddIlm (const Ptr<MplsInterface> &interface, const Ptr<MplsIlm> &ilm);
  /**
   * Add ftn entry
   */
  void AddFtn (const Ptr<MplsFtn> &ftn);

private:
  typedef std::list<Ptr<MplsIlm> > m_ilmTable;
  typedef std::list<Ptr<MplsFtn> > m_ftnTable;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
