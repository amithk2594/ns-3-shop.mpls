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

#ifndef MPLS_FEC_H
#define MPLS_FEC_H

#include <ostream>
#include <list>

#include "ns3/callback.h"
#include "ns3/packet-information.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * Fec is abstract FEC class.
 */
class Fec
{
public:
  Fec ();
  virtual ~Fec ();
  /*
   * \brief Check if packet match the FEC
   * \param pi Packet information 
   */
  virtual bool Match (const PacketInformation* pi) const = 0;
};

//std::ostream& operator<< (std::ostream& os, const Fec& fec);


// Fecs

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
