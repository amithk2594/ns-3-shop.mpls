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
 *         Stefano Avallone <stavallo@gmail.com>
 */

#ifndef MPLS_FEC_H
#define MPLS_FEC_H

#include <ostream>

#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"

#include "mpls-packet-demux.h"

namespace ns3 {
namespace mpls {

class PacketDemux;

/**
 * \ingroup Mpls
 * \brief
 * Fec is abstract FEC class. It is inherited by Fec rules classes
 */
class Fec : public std::unary_function<PacketDemux, bool>
{
public:
  virtual ~Fec ();
  /**
   * \brief Check if the packet matches the FEC
   * \param pc Packet Context
   */
  virtual bool operator() (PacketDemux &pd) const = 0;

  template <class T>
  static Fec* Build (const T &fec) { return new T(fec); }
};

//std::ostream& operator<< (std::ostream& os, const Fec& fec);

// We need to define compositing function objects that inherits from Fec

template <class Operation1, class Operation2>
class UnaryCompose : public Fec
{
protected:
  Operation1 m_fn1;
  Operation2 m_fn2;

public:
  UnaryCompose (const Operation1& x, const Operation2& y): m_fn1(x), m_fn2(y) {}

  typename Operation1::result_type
  operator() (typename Operation2::argument_type& x) const
  {
    return m_fn1 (m_fn2 (x));
  }
};


template <class Operation1, class Operation2, class Operation3>
class BinaryCompose : public Fec
{
protected:
  Operation1 m_fn1;
  Operation2 m_fn2;
  Operation3 m_fn3;

public:
  BinaryCompose(const Operation1& x, const Operation2& y, const Operation3& z): m_fn1(x), m_fn2(y), m_fn3(z) {}

  typename Operation1::result_type
  operator()(typename Operation2::argument_type& x) const
  {
    return m_fn1 (m_fn2 (x), m_fn3 (x));
  }
};


// Logic Operations

template <class LeftPred, class RightPred>
static BinaryCompose<std::logical_and<bool>, LeftPred, RightPred>
operator&& (const LeftPred &left, const RightPred &right)
{
  return BinaryCompose<std::logical_and<bool>, LeftPred, RightPred> (std::logical_and<bool> (), left, right);
}


template <class LeftPred, class RightPred>
static BinaryCompose<std::logical_or<bool>, LeftPred, RightPred>
operator|| (const LeftPred &left, const RightPred &right)
{
  return BinaryCompose<std::logical_or<bool>, LeftPred, RightPred> (std::logical_or<bool> (), left, right);
}


template <class Pred>
static UnaryCompose<std::logical_not<bool>, Pred>
operator! (const Pred &pred)
{
  return UnaryCompose<std::logical_not<bool>, Pred> (std::logical_not<bool> (), pred);
}


// FEC rules

/**
 * \ingroup Mpls
 * \brief
 * Ipv4SourceAddressPrefix matches a given IPv4 source address prefix
 */
class Ipv4SourceAddressPrefix : public Fec
{
public:
  Ipv4SourceAddressPrefix (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4SourceAddressPrefix (char const *address);

  bool operator() (PacketDemux &pd) const;

private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv4DestinationAddressPrefix matches a given IPv4 destination address prefix
 */
class Ipv4DestinationAddressPrefix : public Fec
{
public:
  Ipv4DestinationAddressPrefix (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4DestinationAddressPrefix (char const *address);

  bool operator() (PacketDemux &pd) const;

private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6SourceAddressPrefix matches a given IPv6 source address prefix
 */
class Ipv6SourceAddressPrefix : public Fec
{
public:
  Ipv6SourceAddressPrefix (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6SourceAddressPrefix (char const *address);

  bool operator() (PacketDemux &pd) const;

private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6DestinationAddressPrefixFec matches a given IPv6 destination address prefix
 */
class Ipv6DestinationAddressPrefix : public Fec
{
public:
  Ipv6DestinationAddressPrefix (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6DestinationAddressPrefix (char const *address);

  bool operator() (PacketDemux &pd) const;

private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePort matches a given UDP source port
 */
class UdpSourcePort : public Fec
{
public:
  UdpSourcePort (uint16_t port);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePortRange matches a given range of UDP source ports
 */
class UdpSourcePortRange : public Fec
{
public:
  UdpSourcePortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPort matches a given UDP destination port
 */
class UdpDestinationPort : public Fec
{
public:
  UdpDestinationPort (uint16_t port);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPortRange matches a given range of UDP destination ports
 */
class UdpDestinationPortRange : public Fec
{
public:
  UdpDestinationPortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePort matches a given TCP source port
 */
class TcpSourcePort : public Fec
{
public:
  TcpSourcePort (uint16_t port);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePortRange matches a given range of TCP source ports
 */
class TcpSourcePortRange : public Fec
{
public:
  TcpSourcePortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPort matches a given TCP destination port
 */
class TcpDestinationPort : public Fec
{
public:
  TcpDestinationPort (uint16_t port);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPortRange matches a given range of TCP destination ports
 */
class TcpDestinationPortRange : public Fec
{
public:
  TcpDestinationPortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;

private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
