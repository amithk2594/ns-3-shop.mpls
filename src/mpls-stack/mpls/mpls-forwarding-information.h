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

#ifndef MPLS_FORWARDING_INFORMATION_H
#define MPLS_FORWARDING_INFORMATION_H

#include <list>
#include <ostream>

#include "ns3/simple-ref-count.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief A MPLS forwarding information base class.
 *
 * This is a base class for a MplsIlm and MplsFtn.
 */
class MplsForwardingInformation : public SimpleRefCount<MplsForwardingInformation>
{
public:
  /**
   * \brief Destructor
   */
  virtual ~MplsForwardingInformation ();
  /**
   * \brief Add new NHLFE to the list
   * \param nhlfe MplsNhlfe
   */
  void AddNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove the specific NHLFE from the list
   * \param nhlfe MplsNhlfe
   */
  void RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Print NHLFE
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  MplsForwardingInformation ();

private:
  typedef std::list<Ptr<MplsNhlfe> > NhlfeList;
  NhlfeList m_nhlfeList;
};

/**
 * \brief output operation for MplsForwardingInformation
 */
std::ostream& operator<< (std::ostream& os, const MplsForwardingInformation &info);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_H */
