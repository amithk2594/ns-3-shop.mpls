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

#include "ns3/callback.h"
#include "ns3/packet-information.h"

namespace ns3 {
namespace mpls {

/**
 * \ingroup Mpls
 * \brief
 * Fec is abstract FEC class. It is inherited by two kinds of classes:
 * - FEC rules (which need copy constructor and destructor)
 * - Logic operations (which need default copy constructor and destructor)
 */
class Fec
{
public:
  Fec ();
  virtual ~Fec ();
  /**
   * \brief Check if the packet matches the FEC
   * \param pc Packet Context
   */
  virtual bool Match (const PacketContext* pc) const = 0;
  /**
   * \brief Virtual copy constructor
   */
  virtual Fec* Copy () const = 0;
  /**
   * \brief Remove data allocated for the FEC
   */
  virtual void Remove () const = 0;
};

//std::ostream& operator<< (std::ostream& os, const Fec& fec);


// FEC rules

/**
 * \ingroup Mpls
 * \brief
 * Ipv4SourceAddressPrefixFec matches a given IPv4 source address prefix
 */
class Ipv4SourceAddressPrefixFec : public Fec
{
public:
  Ipv4SourceAddressPrefixFec (const Ipv4Address &address);
  Ipv4SourceAddressPrefixFec (const Ipv4Address &address, uint8_t prefix);
  Ipv4SourceAddressPrefixFec (char const *address);

  Ipv4SourceAddressPrefixFec (const Ipv4SourceAddressPrefixFec &fec);
  
  virtual ~Ipv4SourceAddressPrefixFec ();
  
  /**
   * \brief Get the IP address of the FEC
   */
  Ipv4Address GetAddress (void) const;
  /**
   * \brief Get the prefix of the FEC
   */
  uint8_t GetPrefix (void) const;
  /**
   * \brief Check if the packet matches the FEC
   * \param pc Packet Context
   */
  virtual bool Match (const PacketContext* pc) const;
  /**
   * \brief Virtual copy constructor
   */
  virtual Ipv4SourceAddressPrefixFec* Copy () const;
  /**
   * \brief Remove data allocated for the FEC
   */
  virtual void Remove () const;
  
private:
  Ipv4Address m_address;
  uint8_t m_prefix;
};


// FEC logic operations

/**
 * \ingroup Mpls
 * \brief
 * FecAnd keeps pointers to two FEC rules that must match both
 */
class FecAnd : public Fec
{
public:
  FecAnd (const Fec &left, const Fec &right);

  /**
   * \brief Check if the packet matches both FEC rules
   * \param pc Packet Context
   */
  virtual bool Match (const PacketContext* pc) const;
  /**
   * \brief Virtual copy constructor
   */
  virtual FecAnd* Copy () const;
  /**
   * \brief Remove data allocated for the FEC
   */
  virtual void Remove () const;
  
private:
  Fec* m_left;
  Fec* m_right;
};

FecAnd operator &&(const Fec &left, const Fec &right);


/**
 * \ingroup Mpls
 * \brief
 * FecOr keeps pointers to two FEC rules that must match either
 */
class FecOr : public Fec
{
public:
  FecOr (const Fec &left, const Fec &right);

  /**
   * \brief Check if the packet matches either FEC rules
   * \param pc Packet Context
   */
  virtual bool Match (const PacketContext* pc) const;
  /**
   * \brief Virtual copy constructor
   */
  virtual FecOr* Copy () const;
  /**
   * \brief Remove data allocated for the FEC
   */
  virtual void Remove () const;
  
private:
  Fec* m_left;
  Fec* m_right;
};

FecOr operator ||(const Fec &left, const Fec &right);


/**
 * \ingroup Mpls
 * \brief
 * FecNot keeps a pointer to the FEC rule that must not match
 */
class FecNot : public Fec
{
public:
  FecNot (const Fec &fec);

  /**
   * \brief Check if the packet does not match the FEC rule
   * \param pc Packet Context
   */
  virtual bool Match (const PacketContext* pc) const;
  /**
   * \brief Virtual copy constructor
   */
  virtual FecNot* Copy () const;
  /**
   * \brief Remove data allocated for the FEC
   */
  virtual void Remove () const;
  
private:
  Fec* m_fec;
};

FecNot operator !(const Fec &fec);


} // namespace mpls
} // namespace ns3

#endif /* MPLS_FEC_H */
