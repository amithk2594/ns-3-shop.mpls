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

#ifndef MPLS_FEC_TO_NHLFE_H
#define MPLS_FEC_TO_NHLFE_H

#include <ostream>
#include <stdint.h>

#include "mpls-interface.h"
#include "mpls-forwarding-information.h"
#include "mpls-forwarding-information-base.h"
#include "mpls-fec.h"

namespace ns3 {
namespace mpls {

class ForwardingInformationBase;

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
   * @brief Constructor
   */
  template <class T>
  FecToNhlfe (const T& fec);
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
  template <class T>
  void SetFec (const T& fec);
  /**
   * @brief Print FTN
   * @param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  Fec* m_fec;
  friend class ForwardingInformationBase;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_TO_NHLFE_H */
