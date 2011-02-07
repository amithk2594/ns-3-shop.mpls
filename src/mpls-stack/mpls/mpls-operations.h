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

#ifndef MPLS_OPERATIONS_H
#define MPLS_OPERATIONS_H

#include <ostream>
#include <vector>

#include "ns3/simple-ref-count.h"
#include "ns3/ptr.h"
#include "mpls-generic.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief Mpls label forwarding operation vector
 *
 * This class is used for creating operation sequence
 */
class MplsOp : public SimpleRefCount<MplsOp>
{
public:
  static const int32_t PUSH;
  static const int32_t POP;
  static const int32_t SWAP;
  typedef std::vector<int32_t> Vector;

public:
  /**
   * \brief Create empty operation vector
   */
  MplsOp ();
  /**
   * \brief Destructor
   */
  virtual ~MplsOp ();
  /**
   * \brief Returns operation vector
   */
  virtual MplsOp::Vector& GetOpVector ();

private:
  Vector m_operations;
};


namespace op {

/**
 * \ingroup mpls
 * \brief Push operation
 */
void Push (Ptr<MplsOp> &v, Label label);

/**
 * \ingroup mpls
 * \brief Pop operation.
 */
void Pop (Ptr<MplsOp> &v);

/**
 * \ingroup mpls
 * \brief Swap operation.
 */
void Swap (Ptr<MplsOp> &v, Label label);

} // namespace op 

} // namespace mpls
} // namespace ns3

#endif /* MPLS_OPERATIONS_H */
