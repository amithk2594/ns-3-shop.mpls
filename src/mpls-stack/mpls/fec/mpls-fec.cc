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

#include <iomanip>
#include "mpls-fec.h"

namespace ns3 {
namespace mpls {

Fec::Fec ()
{
}

Fec::~Fec ()
{
}

// std::ostream& 
// operator<< (std::ostream& os, const Fec& fec)
// {
    // // 
    // return os;
// }


Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (const Ipv4Address &address)
{
  m_address = address;
  m_prefix = 32;
}

Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (const Ipv4Address &address, uint8_t prefix)
{
  NS_ASSERT (prefix <= 32);
  m_address = address;
  m_prefix = prefix;
}

Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (char const *address)
{
  NS_ASSERT (strlen (address) < 19);
  char tmp[19];
  char ch = 0;
  char *tp = tmp;
  uint8_t plen = 32;

  while ((ch = *address++) != '\0')
    {
      if (ch == '/')
        {
          plen = static_cast<uint32_t> (atoi (address));
          break;
        }
      else
        {
          *tp++ = ch;
        }
    }
  *tp = 0;

  m_address = Ipv4Address (tmp);
  m_prefix = plen;
}

Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (const Ipv4SourceAddressPrefixFec &fec)
{
  m_address = fec.m_address;
  m_prefix = fec.m_prefix;
}

Ipv4SourceAddressPrefixFec::~Ipv4SourceAddressPrefixFec ()
{
}

Ipv4Address
Ipv4SourceAddressPrefixFec::GetAddress (void) const
{
  return m_address;
}

uint8_t
Ipv4SourceAddressPrefixFec::GetPrefix (void) const
{
  return m_prefix;
}

bool
Ipv4SourceAddressPrefixFec::Match(const PacketContext* pc) const
{
  uint8_t match = 32 - m_prefix;
  
  ProtocolInformation* pi = pc->GetInfo (Ipv4Info::GetType ());
  
  if ((pi) && ((m_address.Get () >> match) == (pi->GetSource ().Get () >> match)))
    {
      return true;
    }
  
  return false;
}

Ipv4SourceAddressPrefixFec*
Ipv4SourceAddressPrefixFec::Copy () const
{
  return new Ipv4SourceAddressPrefixFec (*this);
}

void
Ipv4SourceAddressPrefixFec::Remove() const
{
}


FecAnd::FecAnd(const Fec &left, const Fec &right): m_left (left.Copy ()), m_right (right.Copy ())
{
}

bool
FecAnd::Match(const PacketContext* pc) const
{
  return m_left->Match (pc) && m_right->Match (pc);
}

FecAnd*
FecAnd::Copy() const
{
  return new FecAnd(*this);
}

void
FecAnd::Remove() const
{
  m_left->Remove ();
  m_right->Remove ();
}

FecAnd
operator &&(const Fec &left, const Fec &right)
{
  return FecAnd (left,right);
}



FecOr::FecOr(const Fec &left, const Fec &right): m_left (left.Copy ()), m_right (right.Copy ())
{
}

bool
FecOr::Match(const PacketContext* pc) const
{
  return m_left->Match (pc) || m_right->Match (pc);
}

FecOr*
FecOr::Copy() const
{
  return new FecOr(*this);
}

void
FecOr::Remove() const
{
  m_left->Remove ();
  m_right->Remove ();
}

FecOr
operator &&(const Fec &left, const Fec &right)
{
  return FecOr (left,right);
}


FecNot::FecNot(const Fec &fec): m_fec (fec.Copy ())
{
}

bool
FecNot::Match(const PacketContext* pc) const
{
  return !(m_fec->Match (pc));
}

FecNot*
FecNot::Copy() const
{
  return new FecNot(*this);
}

void
FecNot::Remove() const
{
  m_fec->Remove ();
}

FecNot
operator !(const Fec &fec)
{
  return FecNot (fec);
}

  
} // namespace mpls
} // namespace ns3
