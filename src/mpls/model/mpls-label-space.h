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

#ifndef MPLS_LABEL_SPACE_H
#define MPLS_LABEL_SPACE_H

#include <ostream>
#include <list>
#include <stdint.h>

#include "ns3/simple-ref-count.h"

#include "mpls-label.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief LabelSpace represents an mpls label space
 */
class LabelSpace: public SimpleRefCount<LabelSpace>
{
public:
  /**
   * @brief Create label space
   */
  LabelSpace (uint32_t min, uint32_t max);
  /**
   * @brief Destructor
   */
  virtual ~LabelSpace ();
  /**
   * @brief Allocate label
   */
  Label Allocate ();
  /**
   * @brief Allocate label
   */
  void Deallocate (const Label &label);
  /**
   * @brief Clear table
   */
  void Clear (void);
  
private:
  typedef std::pair<uint32_t, uint32_t> LabelRange;
  typedef std::list<LabelRange> LabelRangeList;  
  
  LabelRangeList m_ranges;
  uint32_t m_min;
  uint32_t m_max;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_LABEL_SPACE_H */
