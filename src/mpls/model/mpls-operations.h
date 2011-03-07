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
#include <stdint.h>

#include "mpls-label.h"

namespace ns3 {
namespace mpls {

typedef std::vector<int32_t> OperationVector;

const uint32_t OP_PUSH = 0;
const uint32_t OP_POP  = 1;
const uint32_t OP_SWAP = 2;

/**
 * \ingroup mpls
 * \brief Mpls label forwarding operation vector
 *
 * This class is used for creating operation sequence
 */
class OperationBuilder
{
public:
  OperationBuilder ();
  ~OperationBuilder ();

  /**
   * @brief Push operation
   */
  OperationBuilder& Push (Label label);
  /**
   * @brief Pop operation.
   */
  OperationBuilder& Pop (void);
  /**
   * @brief Swap operation.
   */
  OperationBuilder& Swap (Label label);
	/**
	 * @brief Returns operations
	 */
	const OperationVector& GetOperations (void) const;
private:
  OperationVector m_operations;  
};


class OperationIterator
{
public:
  OperationIterator (const OperationVector& op);
  ~OperationIterator ();
  bool HasNext () const;
  uint32_t Get ();
  
private:
  OperationVector::const_iterator m_start;
  OperationVector::const_iterator m_end;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_OPERATIONS_H */
