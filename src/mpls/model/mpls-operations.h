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

#ifndef MPLS_OPERATIONS_H
#define MPLS_OPERATIONS_H

#include <stdint.h>

#include "mpls-label.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

const uint32_t OP_POP = 0;
const uint32_t OP_SWAP = 1;

class Nhlfe;
/**
 * \ingroup mpls
 * \brief Base class for label stack operation
 */
class Operation
{
public:
  virtual ~Operation ();
  virtual void Accept (Nhlfe& nhlfe) const = 0;
};

/**
 * \ingroup mpls
 * \brief Pop operation
 */
class Pop : public Operation
{
public:
  Pop ();
  virtual ~Pop ();
  virtual void Accept (Nhlfe& nhlfe) const;
};

/**
 * \ingroup mpls
 * \brief Swap operation
 */
class Swap : public Operation
{
public:
  Swap (Label label1);
  Swap (Label label1, Label label2);
  Swap (Label label1, Label label2, Label label3);
  Swap (Label label1, Label label2, Label label3, Label label4);
  Swap (Label label1, Label label2, Label label3, Label label4, Label label5);
  Swap (Label label1, Label label2, Label label3, Label label4, Label label5, Label label6);
  virtual ~Swap ();
  virtual void Accept (Nhlfe& nhlfe) const;

protected:
  uint32_t m_count;
  uint32_t m_labels[6];
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_OPERATIONS_H */
