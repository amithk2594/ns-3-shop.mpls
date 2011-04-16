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

#ifndef COMMON_CR_TLV_H
#define COMMON_CR_TLV_H

#include <ostream>

#include "ns3/ipv4-address.h"
#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

/**
 * \ingroup ldp
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|0|       Type = 0x0821       |      Length = 4               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |       Reserved        |ActFlg |      Local CR-LSP ID          |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                       Ingress LSR Router ID                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

class LspIdTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  LspIdTLV ();
  LspIdTLV (bool setup, uint16_t lspid, uint32_t routerid);
  virtual ~LspIdTLV ();

  void SetSetup (bool setup);
  bool IsSetup (void) const;
  void SetLspId (uint16_t lspid);
  uint16_t GetLspId (void) const;
  void SetRouterId (uint32_t routerid);
  uint32_t GetRouterId (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  bool m_setup;
  uint16_t m_lspid;
  uint32_t m_routerid;
};

/**
 * \ingroup Ldp
 * Each ER-Hop TLV has the form:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|0|                 Type      |      Length                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |L|                                  Content //                 |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class ExplicitRouteHopTLV : public TypeLengthValue
{
public:
  ExplicitRouteHopTLV ();
  virtual ~ExplicitRouteHopTLV ();

  /**
   * \param loose loose bit attribute value
   * If the value of the ER-Hop attribute is loose then it is a "loose ER-Hop.".
   * Otherwise, it's a "strict ER-Hop."
   */
  void SetLoose (bool loose);
  /**
   * \returns Is ER-hop is loose
   */
  bool IsLoose (void) const;

  virtual Ptr<TypeLengthValue> CopyValue (void) const = 0;

protected:
  bool m_loose;
};

/**
 * \ingroup Ldp
 * The ER-TLV is an object that specifies the path to be taken by the
 * LSP being established.  It is composed of one or more Explicit Route
 * Hop TLVs (ER-Hop TLVs)
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|0|         Type = 0x0800     |      Length                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          ER-Hop TLV 1                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          ER-Hop TLV 2                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  ~                          ............                         ~
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          ER-Hop TLV n                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * Read more: http://www.faqs.org/rfcs/rfc3212.html
 */
class ExplicitRouteTLV : public TypeLengthValue
{
public:
  /**
   * \brief TLV type
   */
  static const uint16_t TLV_TYPE;
  /**
   * \returns this class instance
   */
  static Ptr<TypeLengthValue> Factory (void);

  ExplicitRouteTLV ();
  virtual ~ExplicitRouteTLV ();

  /**
   * \param routeHop add ER-HOP TLV
   */
  void AddRouteHop (Ptr<const ExplicitRouteHopTLV> routeHop);
  /**
   * \returns first route hop or 0
   */
  Ptr<const ExplicitRouteHopTLV> GetFirstRouteHop (void) const;
  /**
   * \returns second route hop or 0
   */
  Ptr<const ExplicitRouteHopTLV> GetSecondRouteHop (void) const;
  /**
   * \brief removing first route hop
   */
  void RemoveFirstRouteHop (void);

  typedef std::list<Ptr<const ExplicitRouteHopTLV> >::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;

  Ptr<ExplicitRouteTLV> Copy (void) const;

  // Functions defined in base class TypeLengthValue
  virtual Ptr<TypeLengthValue> CopyValue (void) const;
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  typedef std::list<Ptr<const ExplicitRouteHopTLV> > RouteHopsList;
  RouteHopsList m_routeHops;
};

/**
 * \ingroup Ldp
 * ER-Hop 1: The IPv4 prefix
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|0|         Type = 0x0801     |      Length = 8               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |L|      Reserved                               |    PreLen     |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                    IPv4 Address (4 bytes)                     |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class Ipv4ExplicitRouteHopTLV : public ExplicitRouteHopTLV
{
public:
  static const uint16_t TLV_TYPE = 0x0801;

  Ipv4ExplicitRouteHopTLV ();
  Ipv4ExplicitRouteHopTLV (const Ipv4Address &address, uint8_t prefix);

  virtual ~Ipv4ExplicitRouteHopTLV ();

  /**
   * \param address Ipv4Address
   * Assign IpvAddress
   */
  void SetAddress (const Ipv4Address &address);
  /**
   * \returns assigned Ipv4 address
   */
  Ipv4Address GetAddress (void) const;
  /**
   * \param prefix
   * Prefix length
   */
  void SetPrefix (uint8_t prefix);
  /**
   * \returns Prefix length
   */
  uint8_t GetPrefix (void) const;

  Ptr<Ipv4ExplicitRouteHopTLV> Copy (void) const;

  // Functions defined in base class TypeLengthValue
  virtual Ptr<TypeLengthValue> CopyValue (void) const;
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint8_t m_prefix;
  Ipv4Address m_address;
};

} // namespace ldp
} // namespace ns3

#endif /* COMMON_CR_TLV_H */
