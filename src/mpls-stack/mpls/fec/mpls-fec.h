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
#include <list>

#include "ns3/callback.h"
#include "ns3/packet-information.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * Fec is abstract FEC class. It is inherited by Fec rules classes
 */
class Fec : public std::unary_function<CommonPacketData, bool>
{
public:
  virtual ~Fec ();
  /**
   * \brief Check if the packet matches the FEC
   * \param pc Packet Context
   */
  virtual bool operator()(const CommonPacketData &pd) const = 0;
};

//std::ostream& operator<< (std::ostream& os, const Fec& fec);

// We need to define compositing function objects that inherits from Fec

template <class _Operation1, class _Operation2>
class unary_compose : public Fec
{
protected:
  _Operation1 _M_fn1;
  _Operation2 _M_fn2;

public:
  unary_compose(const _Operation1& __x, const _Operation2& __y)
    : _M_fn1(__x), _M_fn2(__y)
  {}

  typename _Operation1::result_type
  operator()(const typename _Operation2::argument_type& __x) const
  { 
    return _M_fn1(_M_fn2(__x));
  }
};


template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose : public Fec
{
protected:
  _Operation1 _M_fn1;
  _Operation2 _M_fn2;
  _Operation3 _M_fn3;
  
public:
  binary_compose(const _Operation1& __x, const _Operation2& __y,
		  const _Operation3& __z)
    : _M_fn1(__x), _M_fn2(__y), _M_fn3(__z)
  {}

  typename _Operation1::result_type
  operator()(const typename _Operation2::argument_type& __x) const
  {
    return _M_fn1(_M_fn2(__x), _M_fn3(__x)); 
  }
};


// Logic Operations

template <class LeftPred, class RightPred>
binary_compose<std::logical_and<bool>, LeftPred, RightPred> 
operator &&(const LeftPred &left, const RightPred &right)
{
  return binary_compose<std::logical_and<bool>, LeftPred, RightPred>(std::logical_and<bool>(), left, right);
}


template <class LeftPred, class RightPred>
binary_compose<std::logical_or<bool>, LeftPred, RightPred> 
operator ||(const LeftPred &left, const RightPred &right)
{
  return binary_compose<std::logical_or<bool>, LeftPred, RightPred>(std::logical_or<bool>(), left, right);
}


template <class Pred>
unary_compose<std::logical_not<bool>, Pred> 
operator !(const Pred &pred)
{
  return unary_compose<std::logical_not<bool>, Pred>(std::logical_not<bool>(), pred);
}


// FEC rules

/**
 * \ingroup Mpls
 * \brief
 * Ipv4SourceAddressPrefixFec matches a given IPv4 source address prefix
 */
class Ipv4SourceAddressPrefixFec : public Fec
{
public:
  Ipv4SourceAddressPrefixFec (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4SourceAddressPrefixFec (char const *address);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv4DestinationAddressPrefixFec matches a given IPv4 destination address prefix
 */
class Ipv4DestinationAddressPrefixFec : public Fec
{
public:
  Ipv4DestinationAddressPrefixFec (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4DestinationAddressPrefixFec (char const *address);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6SourceAddressPrefixFec matches a given IPv6 source address prefix
 */
class Ipv6SourceAddressPrefixFec : public Fec
{
public:
  Ipv6SourceAddressPrefixFec (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6SourceAddressPrefixFec (char const *address);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6DestinationAddressPrefixFec matches a given IPv6 destination address prefix
 */
class Ipv6DestinationAddressPrefixFec : public Fec
{
public:
  Ipv6DestinationAddressPrefixFec (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6DestinationAddressPrefixFec (char const *address);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePortFec matches a given UDP source port
 */
class UdpSourcePortFec : public Fec
{
public:
  UdpSourcePortFec (uint16_t port);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePortRangeFec matches a given range of UDP source ports
 */
class UdpSourcePortRangeFec : public Fec
{
public:
  UdpSourcePortRangeFec (uint16_t minport, uint16_t maxport);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_minport;
  uint16_t m_maxport;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPortFec matches a given UDP destination port
 */
class UdpDestinationPortFec : public Fec
{
public:
  UdpDestinationPortFec (uint16_t port);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPortRangeFec matches a given range of UDP destination ports
 */
class UdpDestinationPortRangeFec : public Fec
{
public:
  UdpDestinationPortRangeFec (uint16_t minport, uint16_t maxport);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_minport;
  uint16_t m_maxport;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePortFec matches a given TCP source port
 */
class TcpSourcePortFec : public Fec
{
public:
  TcpSourcePortFec (uint16_t port);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePortRangeFec matches a given range of TCP source ports
 */
class TcpSourcePortRangeFec : public Fec
{
public:
  TcpSourcePortRangeFec (uint16_t minport, uint16_t maxport);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_minport;
  uint16_t m_maxport;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPortFec matches a given TCP destination port
 */
class TcpDestinationPortFec : public Fec
{
public:
  TcpDestinationPortFec (uint16_t port);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPortRangeFec matches a given range of TCP destination ports
 */
class TcpDestinationPortRangeFec : public Fec
{
public:
  TcpDestinationPortRangeFec (uint16_t minport, uint16_t maxport);

  bool operator() (const CommonPacketData &pd) const;
  
private:
  uint16_t m_minport;
  uint16_t m_maxport;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
