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

#ifndef MPLS_FEC_TO_NHLFE_H
#define MPLS_FEC_TO_NHLFE_H

#include <ostream>

#include "mpls-fec.h"
#include "mpls-forwarding-information.h"
#include "mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup mpls
 * \brief
 * The "FEC-to-NHLFE" (FTN) maps each FEC to a set of NHLFEs.
 * It is used when forwarding packets that arrive unlabeled, but which are to be labeled before being forwarded.
 * For more infomation see RFC 3031 (http://www.ietf.org/rfc/rfc3031.txt)
 */
class FecToNhlfe : public ForwardingInformation
{
public:
  /**
   * @brief Construct FTN for specified fec
   * @param fec forwarding equivalence class
   * @param nhlfe NHLFE (at least one NHLFE should be set)
   */
  FecToNhlfe (Fec *fec, const Nhlfe &nhlfe);
  /**
   * @brief Destructor
   */
  virtual ~FecToNhlfe ();
  /**
   * @brief Returns FEC
   */
  const Fec& GetFec (void) const;
  /**
   * @brief Set new FEC
   */
  void SetFec (Fec* fec);
  /**
   * @brief Print FTN
   * @param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  Fec* m_fec;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_TO_NHLFE_H */
