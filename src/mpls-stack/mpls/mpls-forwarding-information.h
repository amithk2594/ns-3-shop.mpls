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

#include "ns3/simple-ref-count.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief
 * This ForwardingInformation class is the base class for the MplsFtn and MplsIlm classes.
 */
class MplsForwardingInformation : public SimpleRefCount<MplsForwardingInformation>
{
public:
  virtual ~MplsForwardingInformation ();
  /**
   * \brief add new NHLFE to the list
   * \param nhlfe
   */
  void AddNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove the specific NHLFE from the list
   * \param nhlfe to remove
   */
  void RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe);
  /**
   * \brief Remove NHLFE from the list with specified lsp id
   * \param lspid
   */
  void RemoveNhlfe (uint16_t lspid);
  /**
   * \brief Get NHLFE with specified lsp id
   * \param lspid
   */
  Ptr<MplsNhlfe> GetNhlfe (uint16_t lspid);
  /**
   * \returns true if information is invalid
   */
  virtual bool IsInvalid (void) const;
  /**
   * \brief print NHLFE
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  /**
   * \brief construct empty object
   */
  MplsForwardingInformation ();

private:
  typedef std::list<Ptr<MplsNhlfe> > NhlfeList;
  NhlfeList m_nhlfeList;
};

std::ostream& operator<< (std::ostream& os, const MplsForwardingInformation &nhlfe);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_H */
