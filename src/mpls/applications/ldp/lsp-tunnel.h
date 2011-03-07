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

#ifndef LSP_TUNNEL_H
#define LSP_TUNNEL_H

#include "ns3/object.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {
namespace ldp {

class LspTunnel : public SimpleRefCount<LspTunnel>
{
public:
  LspTunnel ();
  LspTunnel (uint16_t lspid);
  virtual ~LspTunnel ();
  /**
   * \param lspid tunnel id
   */
  void SetLspId (uint16_t lspid);
  /**
   * \returns LSP tunnel identifier
   */
  uint16_t GetLspId (void) const;
  /**
   * \param label outgoing label
   */
  void SetOutLabel (uint32_t label);
  /**
   * \returns LSP tunnel identifier
   */
  uint32_t GetOutLabel (void) const;
  /**
   * \param ifindex outgoing interface
   */
  void SetOutIfIndex (uint32_t ifIndex);
  /**
   * \returns outgouing interface
   */
  uint32_t GetOutIfIndex (void) const;

  void Print (std::ostream &os) const;

private:
  uint16_t m_lspid;
  uint32_t m_outLabel;
  uint32_t m_outIfIndex;
};

} // namespace ldp
} // namespace ns3

#endif /* LSP_TUNNEL_H */
