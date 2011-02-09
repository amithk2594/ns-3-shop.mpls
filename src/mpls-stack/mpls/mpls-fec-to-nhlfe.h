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

#ifndef MPLS_FTN_H
#define MPLS_FTN_H

#include <ostream>
#include <stdint.h>

#include "mpls-interface.h"
#include "mpls-forwarding-information.h"
#include "mpls-forwarding-information-base.h"
#include "mpls-forwarding-equivalence-class.h"

namespace ns3 {
namespace mpls {

class ForwardingInformation;

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
   * \brief Constructor
   */
  FecToNhlfe (const Ptr<ForwardingEquivalenceClass> &fec);
  /**
   * \brief Destructor
   */
  virtual ~FecToNhlfe ();
  /**
   * \brief
   * \returns
   */
  const Ptr<ForwardingEquivalenceClass>& GetFec (void) const;
  /**
   * \brief Print ILM
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os) const;

private:
  Ptr<ForwardingEquivalenceClass> m_fec;
  friend class ForwardingInformation;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FTN_H */