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

#ifndef COMMON_TLV_H
#define COMMON_TLV_H

#include <ostream>

#include "ns3/ipv4-address.h"

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |0|0| Hop Count (0x0103)        |      Length                   |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     HC Value  |
//   +-+-+-+-+-+-+-+-+
class HopCountTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  HopCountTLV ();
  HopCountTLV (uint8_t value);
  virtual ~HopCountTLV ();

  void SetValue (uint8_t value);
  uint8_t GetValue (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint8_t m_value;
};

//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |0|0| Generic Label (0x0200)    |      Length                   |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Label                                                     |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

class GenericLabelTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  GenericLabelTLV ();
  GenericLabelTLV (uint32_t label);
  virtual ~GenericLabelTLV ();

  void SetLabel (uint32_t label);
  uint32_t GetLabel (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint32_t m_label;
};

//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |0|0| Generic Label (0x0200)    |      Length                   |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Label                                                     |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

class MessageIdTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  MessageIdTLV ();
  MessageIdTLV (uint32_t msgid);
  virtual ~MessageIdTLV ();

  void SetMessageId (uint32_t msgid);
  uint32_t GetMessageId (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint32_t m_msgid;
};
/**
 * \ingroup Ldp
 * Ipv4 Transport Address TLV
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |0|0| Ipv4 Trans. Addr. (0x0401)|      Length                   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                      Ipv4 Address                             |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class Ipv4TransportAddressTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  Ipv4TransportAddressTLV ();
  Ipv4TransportAddressTLV (const Ipv4Address &address);
  virtual ~Ipv4TransportAddressTLV ();

  void SetAddress (const Ipv4Address &address);
  Ipv4Address GetAddress (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  Ipv4Address m_address;
};

} // namespace ldp
} // namespace ns3

#endif /* COMMON_TLV_H */
