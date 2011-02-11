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

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * ForwardingEquivalenceClassBase is the base class of FEC.
 */
class ForwardingEquivalenceClass: public SimpleRefCount<ForwardingEquivalenceClass>
{
public:
  /**
   * \brief Constructor
   */
  ForwardingEquivalenceClass ();
  /**
   * \brief Destructor
   */
  virtual ~ForwardingEquivalenceClass ();
  /**
   * \brief Check if packet match the FEC
   * \param packet Packet
   * \param header IPv4 header
   */
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const = 0;
  /**
   * \brief Check if packet match the FEC
   * \param packet Packet
   * \param header IPv6 header
   */
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const = 0;
  /**
   * \brief Print FEC
   * \param os the stream to print to
   */
  virtual void Print (std::ostream &os, uint32_t indent) const = 0;
};

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingEquivalenceClass> &fec);

/**
 * \brief
 * 'And' logic composite rule
 */
class MplsListFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief List logic
   */
  enum Logic
  {
    LOGIC_AND,
    LOGIC_OR
  };

  /**
   * \brief Constructor
   * \param logic list logic
   */
  MplsListFec (Logic logic);
  /**
   * \brief Destructor
   */
  virtual ~MplsListFec ();
  /**
   * \brief Add rule
   * \param rule
   */
  void Add (const Ptr<ForwardingEquivalenceClass> &fec);
  /**
   * \brief Remove rule
   * \param rule
   */
  void Remove (const Ptr<ForwardingEquivalenceClass> &fec);

  // Functions defined in base class ForwardingEquivalenceClassBase
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

public:
  static Ptr<MplsListFec> All ();
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6, const Ptr<ForwardingEquivalenceClass> &f7);
  static Ptr<MplsListFec> All (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6, const Ptr<ForwardingEquivalenceClass> &f7, const Ptr<ForwardingEquivalenceClass> &f8);

  static Ptr<MplsListFec> Any ();
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6, const Ptr<ForwardingEquivalenceClass> &f7);
  static Ptr<MplsListFec> Any (const Ptr<ForwardingEquivalenceClass> &f1, const Ptr<ForwardingEquivalenceClass> &f2, const Ptr<ForwardingEquivalenceClass> &f3, const Ptr<ForwardingEquivalenceClass> &f4,
    const Ptr<ForwardingEquivalenceClass> &f5, const Ptr<ForwardingEquivalenceClass> &f6, const Ptr<ForwardingEquivalenceClass> &f7, const Ptr<ForwardingEquivalenceClass> &f8);

private:
  typedef std::list<Ptr<ForwardingEquivalenceClass> > FecList;
  Logic m_logic;
  FecList m_fecs;
};

/**
 * \brief
 * Source address rule (ipv4 or ipv6)
 */
class MplsSourceAddressFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief Constructor
   * \param address source address
   */
  MplsSourceAddressFec (const Ipv4Address &address);
  /**
   * \brief Constructor
   * \param address source address
   */
  MplsSourceAddressFec (const Ipv6Address &address);
  /**
   * \brief Destructor
   */
  virtual ~MplsSourceAddressFec ();
  // Functions defined in base class ForwardingEquivalenceClassBase
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

private:
  Address m_address;
};

/**
 * \brief
 * Source address range rule (ipv4 or ipv6)
 */
class MplsSourceAddressRangeFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief Constructor
   * \param min source address
   * \param max source address
   */
  MplsSourceAddressRangeFec (const Ipv4Address &min, const Ipv4Address &max);
  /**
   * \brief Constructor
   * \param min source address
   * \param max source address
   */
  MplsSourceAddressRangeFec (const Ipv6Address &min, const Ipv6Address &max);
  /**
   * \brief Destructor
   */
  virtual ~MplsSourceAddressRangeFec ();

  // Function defined in base class Rule
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

private:
  Address m_addressMin;
  Address m_addressMax;
};

/**
 * \brief
 * Destination address rule
 */
class MplsDestinationAddressFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief Constructor
   * \param address destination address
   */
  MplsDestinationAddressFec (const Ipv4Address &address);
  /**
   * \brief Constructor
   * \param address destination address
   */
  MplsDestinationAddressFec (const Ipv6Address &address);
  /**
   * \brief Destructor
   */
  virtual ~MplsDestinationAddressFec ();

  // Function defined in base class ForwardingEquivalenceClassBase
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

private:
  Address m_address;
};

/**
 * \brief
 * Ipv4 destination address range rule
 */
class MplsDestinationAddressRangeFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief Constructor
   * \param min source address
   * \param max source address
   */
  MplsDestinationAddressRangeFec (const Ipv4Address &min, const Ipv4Address &max);
  /**
   * \brief Constructor
   * \param min source address
   * \param max source address
   */
  MplsDestinationAddressRangeFec (const Ipv6Address &min, const Ipv6Address &max);
  /**
   * \brief Destructor
   */
  virtual ~MplsDestinationAddressRangeFec ();

  // Function defined in base class Rule
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

private:
  Address m_addressMin;
  Address m_addressMax;
};

/**
 * \brief
 * Protocol rule
 */
class MplsProtocolFec: public ForwardingEquivalenceClass
{
public:
  /**
   * \brief Constructor
   * \param min
   * \param max
   */
  MplsProtocolFec (uint8_t protocol);
  /**
   * \brief Destructor
   */
  virtual ~MplsProtocolFec ();

  // Functions defined in base class Rule
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const;
  virtual bool IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const;
  virtual void Print (std::ostream &os, uint32_t indent) const;

private:
  uint8_t m_protocol;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
