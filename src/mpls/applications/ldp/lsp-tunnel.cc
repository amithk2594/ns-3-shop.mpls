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

#include "ns3/assert.h"

#include "lsp-tunnel.h"

namespace ns3 {
namespace ldp {

LspTunnel::LspTunnel ()
  : m_lspid (0),
    m_outLabel (-1),
    m_outIfIndex (-1)
{
}

LspTunnel::LspTunnel (uint16_t lspid)
  : m_lspid (lspid),
    m_outLabel (-1),
    m_outIfIndex (-1)
{
}

LspTunnel::~LspTunnel ()
{
}

void
LspTunnel::SetLspId (uint16_t lspid)
{
  m_lspid = lspid;
}

uint16_t
LspTunnel::GetLspId (void) const
{
  return m_lspid;
}

void
LspTunnel::SetOutLabel (uint32_t label)
{
  m_outLabel = label;
}

uint32_t
LspTunnel::GetOutLabel (void) const
{
  return m_outLabel;
}

void
LspTunnel::SetOutIfIndex (uint32_t ifIndex)
{
  m_outIfIndex = ifIndex;
}

uint32_t
LspTunnel::GetOutIfIndex (void) const
{
  return m_outIfIndex;
}

void
LspTunnel::Print (std::ostream &os) const
{
  os << "LSPID:" << m_lspid << " oLabel:" << m_outLabel << " oInterface:" << m_outIfIndex;
}

} // namespace ldp
} // namespace ns3
