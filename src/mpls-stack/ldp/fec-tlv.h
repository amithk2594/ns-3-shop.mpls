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


#ifndef FEC_TLV_H
#define FEC_TLV_H

#include <vector>
#include <ostream>

#include "ns3/simple-ref-count.h"
#include "ns3/address.h"

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

class FecElement : public SimpleRefCount<FecElement>
{
public:
  FecElement ();
  virtual ~FecElement ();

  virtual uint8_t GetType (void) const = 0;
  virtual uint32_t GetLength (void) const = 0;
  virtual void SerializeValue (Buffer::Iterator &start) const = 0;
  virtual bool DeserializeValue (Buffer::Iterator start) = 0;
  virtual void Print (std::ostream &os) const = 0;
};


/**
 * \ingroup ldp
 *
 * FEC TLV encoding
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |0|0| FEC (0x0100)              |      Length                   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                        FEC Element 1                          |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   ~                                                               ~
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                        FEC Element n                          |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class FecTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  FecTLV ();
  virtual ~FecTLV ();

  void AddElement (Ptr<const FecElement> element);
  Ptr<const FecElement> GetElement (uint32_t i) const;
  uint32_t GetNElements (void) const;

  typedef std::vector<Ptr<const FecElement> >::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;

  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

  static uint16_t GetAddressFamily (const Address &address);

private:
  typedef std::vector<Ptr<const FecElement> > FecElementVector;
  FecElementVector m_elements;
};

class WildcardFecElement : public FecElement
{
public:
  static const uint8_t FEC_ELEMENT_TYPE;

  WildcardFecElement ();
  virtual ~WildcardFecElement ();

  uint8_t GetType (void) const;
  uint32_t GetLength (void) const;
  void SerializeValue (Buffer::Iterator &start) const;
  bool DeserializeValue (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  static Ptr<FecTLV> CreateFecTLV (void);
};

/**
 * \ingroup ldp
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |  Prefix (2)   |     Address Family            |     PreLen    |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                     Prefix                                    |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class PrefixFecElement : public FecElement
{
public:
  static const uint8_t FEC_ELEMENT_TYPE;

  PrefixFecElement ();
  PrefixFecElement (const Address &address, uint8_t prefix);
  virtual ~PrefixFecElement ();

  void SetAddress (const Address &address);
  Address GetAddress (void) const;

  void SetPrefix (uint8_t prefix);
  uint8_t GetPrefix (void) const;

  uint8_t GetType (void) const;
  uint32_t GetLength (void) const;
  void SerializeValue (Buffer::Iterator &start) const;
  bool DeserializeValue (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  static Ptr<FecTLV> CreateFecTLV (const Address &address, uint8_t prefix);

private:
  uint8_t m_prefix;
  Address m_address;
};

/**
 * \ingroup ldp
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   | Host Addr (3) |     Address Family            | Host Addr Len |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                     Host Addr                                 |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class HostAddressFecElement : public FecElement
{
public:
  static const uint8_t FEC_ELEMENT_TYPE;

  HostAddressFecElement ();
  HostAddressFecElement (const Address &address);
  virtual ~HostAddressFecElement ();

  void SetAddress (const Address &address);
  Address GetAddress (void) const;

  uint8_t GetType (void) const;
  uint32_t GetLength (void) const;
  void SerializeValue (Buffer::Iterator &start) const;
  bool DeserializeValue (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  static Ptr<FecTLV> CreateFecTLV (const Address &address);
private:
  Address m_address;
};

/**
 * \ingroup ldp
 * The CR-LSP FEC TLV encoding is as follows:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|0|          Type = 0x0100    |      Length = 1               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | CR-LSP (4)    |
 *  +-+-+-+-+-+-+-+-+
 */

class CrLspFecElement : public FecElement
{
public:
  static const uint8_t FEC_ELEMENT_TYPE;

  CrLspFecElement ();
  virtual ~CrLspFecElement ();

  uint8_t GetType (void) const;
  uint32_t GetLength (void) const;
  void SerializeValue (Buffer::Iterator &start) const;
  bool DeserializeValue (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  static Ptr<FecTLV> CreateFecTLV (void);
};

} // namespace ldp
} // namespace ns3

#endif /* FEC_TLV_H */
