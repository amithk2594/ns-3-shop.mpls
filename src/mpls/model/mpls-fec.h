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
template <class T> class FecOperand;

/**
 * \ingroup Mpls
 * \brief
 * Fec is abstract FEC class. It is inherited by Fec rules classes
 */
class Fec
{
public:
  virtual ~Fec ();
  /**
   * @brief Check if the packet matches the FEC
   * @param pd PacketDemux object
   */  
  virtual bool operator() (PacketDemux &pd) const = 0;
  virtual void Print (std::ostream &os) const = 0;

  template <class T> static Fec* Build (const T &fec) { return new T(fec); }
};

template <class A, class B>
class __And : public FecOperand<__And<A,B> >
{
protected:
  A m_a;
  B m_b;

public:
  __And (const A& a, const B& b): m_a(a), m_b(b) {}
  
  bool operator() (PacketDemux &pd) const { return m_a (pd) && m_b (pd); }
  void Print (std::ostream &os) const {os << "(" << m_a << " & " << m_b << ")";};
};

template <class A, class B>
class __Or : public FecOperand<__Or<A,B> >
{
protected:
  A m_a;
  B m_b;

public:
  __Or (const A& a, const B& b): m_a(a), m_b(b) {}
  
  bool operator() (PacketDemux &pd) const { return m_a (pd) || m_b (pd); }
  void Print (std::ostream &os) const {os << "(" << m_a << " | " << m_b << ")";};  
};

template <class A>
class __Not : public FecOperand<__Not<A> >
{
protected:
  A m_a;

public:
  __Not (const A& a): m_a(a) {}
  
  bool operator() (PacketDemux &pd) const { return !m_a (pd); }
  void Print (std::ostream &os) const {os << "~" << m_a; };
};

template <class C>
class FecOperand : public Fec
{
public:
  template <class T>
  __And<C, T> operator&& (const T &right) { return __And<C, T> (*(C*)this, right); }
  template <class T>
  __Or<C, T> operator|| (const T &right) { return __Or<C, T> (*(C*)this, right); }
  __Not<C> operator! () { return __Not<C> (*(C*)this); }

  friend std::ostream& operator<< (std::ostream& os, const C& c) { c.Print (os); return os; }
};

//std::ostream& operator<< (std::ostream& os, const Fec& fec);

// FEC rules

/**
 * \ingroup Mpls
 * \brief
 * Ipv4Source matches a given IPv4 source address prefix
 */
class Ipv4Source : public FecOperand<Ipv4Source>
{
public:
  Ipv4Source (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4Source (char const *address);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;

private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv4Destination matches a given IPv4 destination address prefix
 */
class Ipv4Destination : public FecOperand<Ipv4Destination>
{
public:
  Ipv4Destination (const Ipv4Address &address, const Ipv4Mask &mask = Ipv4Mask ("/32"));
  Ipv4Destination (char const *address);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;

private:
  Ipv4Address m_address;
  Ipv4Mask m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6Source matches a given IPv6 source address prefix
 */
class Ipv6Source : public FecOperand<Ipv6Source>
{
public:
  Ipv6Source (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6Source (char const *address);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;  

private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * Ipv6DestinationFec matches a given IPv6 destination address prefix
 */
class Ipv6Destination : public FecOperand<Ipv6Destination>
{
public:
  Ipv6Destination (const Ipv6Address &address, const Ipv6Prefix &mask = Ipv6Prefix (128));
  Ipv6Destination (char const *address);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  Ipv6Address m_address;
  Ipv6Prefix m_mask;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePort matches a given UDP source port
 */
class UdpSourcePort : public FecOperand<UdpSourcePort>
{
public:
  UdpSourcePort (uint16_t port);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpSourcePortRange matches a given range of UDP source ports
 */
class UdpSourcePortRange : public FecOperand<UdpSourcePortRange>
{
public:
  UdpSourcePortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPort matches a given UDP destination port
 */
class UdpDestinationPort : public FecOperand<UdpDestinationPort>
{
public:
  UdpDestinationPort (uint16_t port);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * UdpDestinationPortRange matches a given range of UDP destination ports
 */
class UdpDestinationPortRange : public FecOperand<UdpDestinationPortRange>
{
public:
  UdpDestinationPortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePort matches a given TCP source port
 */
class TcpSourcePort : public FecOperand<TcpSourcePort>
{
public:
  TcpSourcePort (uint16_t port);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpSourcePortRange matches a given range of TCP source ports
 */
class TcpSourcePortRange : public FecOperand<TcpSourcePortRange>
{
public:
  TcpSourcePortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPort matches a given TCP destination port
 */
class TcpDestinationPort : public FecOperand<TcpDestinationPort>
{
public:
  TcpDestinationPort (uint16_t port);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_port;
};


/**
 * \ingroup Mpls
 * \brief
 * TcpDestinationPortRange matches a given range of TCP destination ports
 */
class TcpDestinationPortRange : public FecOperand<TcpDestinationPortRange>
{
public:
  TcpDestinationPortRange (uint16_t minPort, uint16_t maxPort);

  bool operator() (PacketDemux &pd) const;
  void Print (std::ostream &os) const;
  
private:
  uint16_t m_minPort;
  uint16_t m_maxPort;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
