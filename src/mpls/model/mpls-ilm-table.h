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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#ifndef MPLS_FORWARDING_INFORMATION_BASE_H
#define MPLS_FORWARDING_INFORMATION_BASE_H

#include <ostream>
#include <map>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-incoming-label-map.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief IlmTable represents an ILM table
 */
class IlmTable: public SimpleRefCount<IlmTable>
{
private:
  typedef std::map<uint32_t, Ptr<IncomingLabelMap> > IlmList;

public:
  typedef IlmList::const_iterator Iterator;
  
  /**
   * @brief Create empty ILM table
   */
  IlmTable ();
  /**
   * @brief Destructor
   */
  virtual ~IlmTable ();
  /**
   * @brief Add ILM with a single NHLFE
   * @param interface
   * @param label
   * @param nhlfe
   * @return index of the ILM
   */
  uint32_t AddIlm (Ptr<MplsInterface>& interface, Label label, const Nhlfe &nhlfe);
  /**
   * @brief Add interface-independent ILM with a single NHLFE
   * @param label
   * @param nhlfe
   * @return index of the ILM
   */
  uint32_t AddIlm (Label label, const Nhlfe &nhlfe);
  /**
   * @brief Add ILM with (possibly) multiple NHLFEs
   * @param ilm
   * @return index of the ILM
   */
  uint32_t AddIlm (const Ptr<IncomingLabelMap> &ilm);
  /**
   * @brief Get ILM by Index
   * @param index
   * @return ILM identified by index
   */
  Ptr<IncomingLabelMap> GetIlmByIndex (const uint32_t index);
  /**
   * @brief Remove ILM by index
   * @param index
   */
  void RemoveIlmByIndex (const uint32_t index);
  /**
   * \brief Print forwarding information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;
  /**
   * \brief Returns an iterator which refers to the first ILM in the table
   */
  Iterator Begin (void) const;
  /**
   * \brief Returns an iterator which indicates past-the-last ILM in the table
   */
  Iterator End (void) const;
  
private:
  IlmList m_ilmTable;
  static uint32_t ilmId;
};

std::ostream& operator<< (std::ostream& os, const Ptr<IlmTable> &ilm);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FORWARDING_INFORMATION_BASE_H */
