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

#ifndef COMMON_HELLO_PARAMS_TLV_H
#define COMMON_HELLO_PARAMS_TLV_H

#include <ostream>

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

/**
 * \ingroup Ldp
 * Common Hello Parameters TLV
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |0|0| Common Hello Parms(0x0400)|      Length                   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |      Hold Time                |T|R| Reserved                  |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class CommonHelloParamsTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  CommonHelloParamsTLV ();
  virtual ~CommonHelloParamsTLV ();

  /**
   * \param time hold time field value
   */
  void SetHoldTime (uint16_t time);
  /**
   * \returns hole time field value
   */
  uint16_t GetHoldTime (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint16_t m_holdTime;
};

} // namespace ldp
} // namespace ns3

#endif /* COMMON_HELLO_PARAMS_TLV_H */
