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

#ifndef PATH_VECTOR_TLV_H
#define PATH_VECTOR_TLV_H

#include <ostream>
#include <vector>

#include "ldp-header.h"

namespace ns3 {
namespace ldp {

//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |0|0| Path Vector (0x0104)      |        Length                 |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                            LSR Id 1                           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                                                               |
//   ~                                                               ~
//   |                                                               |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                            LSR Id n                           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class PathVectorTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE = 0x0104;

  PathVectorTLV ();
  virtual ~PathVectorTLV ();

  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;

  void AddLsr (uint32_t id);
  uint32_t GetLsr (uint32_t i) const;
  uint32_t GetNLsrs (void) const;

  typedef vector<uint32_t>::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;

  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  typedef vector<uint32_t> LsrsVector;
  LsrVector m_lsrs;
};

}
}  // namespace ns3

#endif
