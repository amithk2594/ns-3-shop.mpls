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

MplsFec::MplsFec ()
{
}

MplsFec::~MplsFec ()
{
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsFec> &fec)
{
  fec->Print (os, 0);
  return os;
}


MplsListFec::MplsListFec (Logic logic)
  : m_logic (logic)
{
}

MplsListFec::~MplsListFec ()
{
}

void
MplsListFec::Add (const Ptr<MplsFec> &fec)
{
  m_fecs.push_back (fec);
}

void
MplsListFec::Remove (const Ptr<MplsFec> &fec)
{
  m_fecs.remove (fec);
}

inline
bool
MplsListFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  bool andLogic = m_logic == LOGIC_AND;
  if (andLogic)
    {
      for (FecList::const_iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
        {
          if (!(*i)->IsMatch (packet, header))
            {
              return false;
            }
        }
    }
  else
    {
      for (FecList::const_iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
        {
          if ((*i)->IsMatch (packet, header))
            {
              return true;
            }
        }
    }
  return andLogic;
}

inline
bool
MplsListFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  bool andLogic = m_logic == LOGIC_AND;
  if (andLogic)
    {
      for (FecList::const_iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
        {
          if (!(*i)->IsMatch (packet, header))
            {
              return false;
            }
        }
    }
  else
    {
      for (FecList::const_iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
        {
          if ((*i)->IsMatch (packet, header))
            {
              return true;
            }
        }
    }
  return andLogic;
}

void
MplsListFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << "";
  if (m_logic == LOGIC_AND)
    {
      os << "All of" << std::endl;
    }
  else
    {
      os << "Any of" << std::endl;
    }
  for (FecList::const_iterator i = m_fecs.begin (); i != m_fecs.end (); ++i)
    {
      (*i)->Print (os, indent + 2);
      os << std::endl;
    }
}

Ptr<MplsListFec>
MplsListFec::All ()
{
  return Create<MplsListFec> (LOGIC_AND);
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6, const Ptr<MplsFec> &f7)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  fec->Add (f7);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::All (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6, const Ptr<MplsFec> &f7, const Ptr<MplsFec> &f8)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_AND);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  fec->Add (f7);
  fec->Add (f8);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any ()
{
  return Create<MplsListFec> (LOGIC_OR);
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6, const Ptr<MplsFec> &f7)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  fec->Add (f7);
  return fec;
}

Ptr<MplsListFec>
MplsListFec::Any (const Ptr<MplsFec> &f1, const Ptr<MplsFec> &f2, const Ptr<MplsFec> &f3, const Ptr<MplsFec> &f4,
    const Ptr<MplsFec> &f5, const Ptr<MplsFec> &f6, const Ptr<MplsFec> &f7, const Ptr<MplsFec> &f8)
{
  Ptr<MplsListFec> fec = Create<MplsListFec> (LOGIC_OR);
  fec->Add (f1);
  fec->Add (f2);
  fec->Add (f3);
  fec->Add (f4);
  fec->Add (f5);
  fec->Add (f6);
  fec->Add (f7);
  fec->Add (f8);
  return fec;
}

MplsSourceAddressFec::MplsSourceAddressFec (const Ipv4Address &address)
  : m_address (address)
{
}

MplsSourceAddressFec::MplsSourceAddressFec (const Ipv6Address &address)
  : m_address (address)
{
}

MplsSourceAddressFec::~MplsSourceAddressFec ()
{
}

inline
bool
MplsSourceAddressFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  return header.GetSource () == m_address;
}

inline
bool
MplsSourceAddressFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  return header.GetSourceAddress () == m_address;
}

void
MplsSourceAddressFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << ""
     << "Source Address: ";
  if (Ipv4Address::IsMatchingType (m_address))
    {
      os << Ipv4Address::ConvertFrom (m_address);
    }
  else if (Ipv6Address::IsMatchingType (m_address))
    {
      os << Ipv6Address::ConvertFrom (m_address);
    }
  else
    {
      os << m_address;
    }
}

MplsSourceAddressRangeFec::MplsSourceAddressRangeFec (const Ipv4Address &min, const Ipv4Address &max)
{
  if (max < min)
    {
      m_addressMin = max;
      m_addressMax = min;
    }
  else
    {
      m_addressMin = min;
      m_addressMax = max;
    }
}

MplsSourceAddressRangeFec::MplsSourceAddressRangeFec (const Ipv6Address &min, const Ipv6Address &max)
{
  if (max < min)
    {
      m_addressMin = max;
      m_addressMax = min;
    }
  else
    {
      m_addressMin = min;
      m_addressMax = max;
    }
}

MplsSourceAddressRangeFec::~MplsSourceAddressRangeFec ()
{
}

inline
bool
MplsSourceAddressRangeFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  Address s = header.GetSource ();
  return (m_addressMin == s || m_addressMin < s) && (m_addressMax == s || s < m_addressMax);
}

inline
bool
MplsSourceAddressRangeFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  Address s = header.GetSourceAddress ();
  return (m_addressMin == s || m_addressMin < s) && (m_addressMax == s || s < m_addressMax);
}

void
MplsSourceAddressRangeFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << ""
     << "Source Address Range: ";
  if (Ipv4Address::IsMatchingType (m_addressMin))
    {
      os << Ipv4Address::ConvertFrom (m_addressMin) << " - " << Ipv4Address::ConvertFrom (m_addressMax);
    }
  else if (Ipv6Address::IsMatchingType (m_addressMin))
    {
      os << Ipv6Address::ConvertFrom (m_addressMin) << " - " << Ipv6Address::ConvertFrom (m_addressMax);
    }
  else
    {
      os << m_addressMin << " - " << m_addressMax;
    }
}

MplsDestinationAddressFec::MplsDestinationAddressFec (const Ipv4Address &address)
  : m_address (address)
{
}

MplsDestinationAddressFec::MplsDestinationAddressFec (const Ipv6Address &address)
  : m_address (address)
{
}

MplsDestinationAddressFec::~MplsDestinationAddressFec ()
{
}

inline
bool
MplsDestinationAddressFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  return header.GetDestination () == m_address;
}

inline
bool
MplsDestinationAddressFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  return header.GetDestinationAddress () == m_address;
}

void
MplsDestinationAddressFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << ""
     << "Destination Address: ";
  if (Ipv4Address::IsMatchingType (m_address))
    {
      os << Ipv4Address::ConvertFrom (m_address);
    }
  else if (Ipv6Address::IsMatchingType (m_address))
    {
      os << Ipv6Address::ConvertFrom (m_address);
    }
  else
    {
      os << m_address;
    }
}

MplsDestinationAddressRangeFec::MplsDestinationAddressRangeFec (const Ipv4Address &min, const Ipv4Address &max)
{
  if (max < min)
    {
      m_addressMin = max;
      m_addressMax = min;
    }
  else
    {
      m_addressMin = min;
      m_addressMax = max;
    }
}

MplsDestinationAddressRangeFec::MplsDestinationAddressRangeFec (const Ipv6Address &min, const Ipv6Address &max)
{
  if (max < min)
    {
      m_addressMin = max;
      m_addressMax = min;
    }
  else
    {
      m_addressMin = min;
      m_addressMax = max;
    }
}

MplsDestinationAddressRangeFec::~MplsDestinationAddressRangeFec ()
{
}

inline
bool
MplsDestinationAddressRangeFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  Address s = header.GetDestination ();
  return (m_addressMin == s || m_addressMin < s) && (m_addressMax == s || s < m_addressMax);
}

inline
bool
MplsDestinationAddressRangeFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  Address s = header.GetDestinationAddress ();
  return (m_addressMin == s || m_addressMin < s) && (m_addressMax == s || s < m_addressMax);
}

void
MplsDestinationAddressRangeFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << ""
     << "Destination Address Range: ";
  if (Ipv4Address::IsMatchingType (m_addressMin))
    {
      os << Ipv4Address::ConvertFrom (m_addressMin) << " - " << Ipv4Address::ConvertFrom (m_addressMax);
    }
  else if (Ipv6Address::IsMatchingType (m_addressMin))
    {
      os << Ipv6Address::ConvertFrom (m_addressMin) << " - " << Ipv6Address::ConvertFrom (m_addressMax);
    }
  else
    {
      os << m_addressMin << " - " << m_addressMax;
    }
}

MplsProtocolFec::MplsProtocolFec (uint8_t protocol)
  : m_protocol (protocol)
{
}

MplsProtocolFec::~MplsProtocolFec ()
{
}

inline
bool
MplsProtocolFec::IsMatch (Ptr<const Packet> packet, const Ipv4Header &header) const
{
  return header.GetProtocol () == m_protocol;
}

inline
bool
MplsProtocolFec::IsMatch (Ptr<const Packet> packet, const Ipv6Header &header) const
{
  return header.GetNextHeader () == m_protocol;
}

void
MplsProtocolFec::Print (std::ostream &os, uint32_t indent) const
{
  os << std::setw(indent) << ""
     << "Protocol: ";
  switch (m_protocol)
  {
    case 17:
      os << "UDP";
      break;
    case 6:
      os << "TCP";
      break;
    case 1: case 58:
      os << "ICMP";
      break;
    default:
      os << (int) m_protocol;
  }
}


} // namespace mpls
} // namespace ns3
