/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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

#ifndef MPLS_FTN_TABLE_H
#define MPLS_FTN_TABLE_H

#include <ostream>
#include <map>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-fec.h"
#include "mpls-nhlfe.h"
#include "mpls-fec-to-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief FtnTable represents a FTN table
 */
class FtnTable: public SimpleRefCount<FtnTable>
{
private:
  typedef std::map<uint32_t, Ptr<FecToNhlfe> > FtnList;

public:
  typedef FtnList::const_iterator Iterator;
  
  /**
   * @brief Create empty FTN table
   */
  FtnTable ();
  /**
   * @brief Destructor
   */
  virtual ~FtnTable ();
  /**
   * @brief Add FTN with (possibly) multiple NHLFEs
   * @param ftn
   * @return index of the FTN
   */
  uint32_t AddFtn (const Ptr<FecToNhlfe> &ftn);
  /**
   * @brief Get FTN by Index
   * @param index
   * @return FTN identified by index
   */
  Ptr<FecToNhlfe> GetFtn (const uint32_t index);
  /**
   * @brief Remove FTN by index
   * @param index
   */
  void RemoveFtn (const uint32_t index);
  /**
   * @brief Clear table
   */
  void Clear (void);  
  /**
   * \brief Print forwarding information
   * \param os the stream to print to
   */
  void Print (std::ostream &os) const;
  /**
   * \brief Returns an iterator which refers to the first FTN in the table
   */
  Iterator Begin (void) const;
  /**
   * \brief Returns an iterator which indicates past-the-last FTN in the table
   */
  Iterator End (void) const;
  
private:
  FtnList m_ftnTable;
  static uint32_t ftnId;
};

std::ostream& operator<< (std::ostream& os, const Ptr<FtnTable> &ftn);

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FTN_TABLE_H */
