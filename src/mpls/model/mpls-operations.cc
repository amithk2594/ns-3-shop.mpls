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

#include "mpls-operations.h"
#include "ns3/assert.h"

namespace ns3 {
namespace mpls {

Operation::~Operation ()
{
}

Pop::Pop ()
{
}

Pop::~Pop ()
{
}

void
Pop::Accept (Nhlfe& nhlfe)
{
  nhlfe.m_opcode = OP_POP;
}

Swap::Swap (Label label1)
  : m_count (1)
{
  m_labels[0] = label1;
}

Swap::Swap (Label label1, Label label2)
  : m_count (2)
{
  m_labels[0] = label1;
  m_labels[1] = label2;
}

Swap::Swap (Label label1, Label label2, Label label3)
  : m_count (3)
{
  m_labels[0] = label1;
  m_labels[1] = label2;
  m_labels[2] = label3;
}

Swap::Swap (Label label1, Label label2, Label label3, Label label4)
  : m_count (4)
{
  m_labels[0] = label1;
  m_labels[1] = label2;
  m_labels[2] = label3;
  m_labels[3] = label4;
}

Swap::Swap (Label label1, Label label2, Label label3, Label label4, 
            Label label5)
  : m_count (5)
{
  m_labels[0] = label1;
  m_labels[1] = label2;
  m_labels[2] = label3;
  m_labels[3] = label4;
  m_labels[4] = label5;
}

Swap::Swap (Label label1, Label label2, Label label3, Label label4, 
            Label label5, Label label6)
  : m_count (6)
{
  m_labels[0] = label1;
  m_labels[1] = label2;
  m_labels[2] = label3;
  m_labels[3] = label4;
  m_labels[4] = label5;
  m_labels[5] = label6;
}

Swap::~Swap ()
{
}

void
Swap::Accept (Nhlfe& nhlfe)
{
  nhlfe.m_opcode = OP_SWAP;
  nhlfe.m_count = m_count;
  for (int i = 0; i < m_count; ++i) {
    nhlfe.m_labels[i] = m_labels[i];
  }
}

} // namespace mpls
} // namespace ns3
