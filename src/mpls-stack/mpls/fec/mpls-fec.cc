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

#include <iomanip>
#include "mpls-fec.h"

namespace ns3 {
namespace mpls {

Fec::~Fec ()
{
}

// std::ostream& 
// operator<< (std::ostream& os, const Fec& fec)
// {
    // // 
    // return os;
// }

template <class Address, class Mask>
static void AsciiToPrefix (char const *addrstr, Address &address, Mask &mask, bool slash)
{
  #define MAX_STR_LEN 80
  
  NS_ASSERT (strlen (addrstr) < MAX_STR_LEN);
  char tmp[MAX_STR_LEN];
  char ch = 0;
  char *tp = tmp;

  while ((ch = *addrstr++) != '\0')
    {
      if (ch == '/')
        {
          mask = Mask (slash ? --addrstr : addrstr);
          break;
        }
      else
        {
          *tp++ = ch;
        }
    }
  *tp = 0;

  address = Address (tmp);
}


Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{}

Ipv4SourceAddressPrefixFec::Ipv4SourceAddressPrefixFec (char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4SourceAddressPrefixFec::operator()(const CommonPacketData &pd) const
{
  Ipv4Header* h = pd.GetIpv4Header ();
  
  if ((h) && (m_mask.IsMatch (h->GetSource (), m_address)))
    {
      return true;
    }
  
  return false;
}


Ipv4DestinationAddressPrefixFec::Ipv4DestinationAddressPrefixFec(const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{}

Ipv4DestinationAddressPrefixFec::Ipv4DestinationAddressPrefixFec(char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4DestinationAddressPrefixFec::operator()(const CommonPacketData &pd) const
{
  Ipv4Header* h = pd.GetIpv4Header ();
  
  if ((h) && (m_mask.IsMatch (h->GetDestination (), m_address)))
    {
      return true;
    }
  
  return false;
}


Ipv6SourceAddressPrefixFec::Ipv6SourceAddressPrefixFec (const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{}

Ipv6SourceAddressPrefixFec::Ipv6SourceAddressPrefixFec (char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6SourceAddressPrefixFec::operator()(const CommonPacketData &pd) const
{
  Ipv6Header* h = pd.GetIpv6Header ();
  
  if ((h) && (m_mask.IsMatch (h->GetSourceAddress (), m_address)))
    {
      return true;
    }
  
  return false;
}

  
Ipv6DestinationAddressPrefixFec::Ipv6DestinationAddressPrefixFec(const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{}

Ipv6DestinationAddressPrefixFec::Ipv6DestinationAddressPrefixFec(char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6DestinationAddressPrefixFec::operator()(const CommonPacketData &pd) const
{
  Ipv6Header* h = pd.GetIpv6Header ();
  
  if ((h) && (m_mask.IsMatch (h->GetDestinationAddress (), m_address)))
    {
      return true;
    }
  
  return false;
}


UdpSourcePortFec::UdpSourcePortFec(uint16_t port)
  : m_port (port)
{}

bool
UdpSourcePortFec::operator()(const CommonPacketData& pd) const
{
  UdpHeader* h = pd.GetUdpHeader ();
  
  if ((h) && (h->GetSourcePort () == m_port))
    {
      return true;
    }
  
  return false;
}



UdpSourcePortRangeFec::UdpSourcePortRangeFec(uint16_t minport, uint16_t maxport)
  : m_minport (minport),
    m_maxport (maxport)
{}

bool
UdpSourcePortRangeFec::operator()(const CommonPacketData& pd) const
{
  UdpHeader* h = pd.GetUdpHeader ();
  
  if ((h) && (h->GetSourcePort () >= m_minport) && (h->GetSourcePort () <= m_maxport))
    {
      return true;
    }
  
  return false;
}


UdpDestinationPortFec::UdpDestinationPortFec(uint16_t port)
  : m_port (port)
{}

bool
UdpDestinationPortFec::operator()(const CommonPacketData& pd) const
{
  UdpHeader* h = pd.GetUdpHeader ();
  
  if ((h) && (h->GetDestinationPort () == m_port))
    {
      return true;
    }
  
  return false;
}



UdpDestinationPortRangeFec::UdpDestinationPortRangeFec(uint16_t minport, uint16_t maxport)
  : m_minport (minport),
    m_maxport (maxport)
{}

bool
UdpDestinationPortRangeFec::operator()(const CommonPacketData& pd) const
{
  UdpHeader* h = pd.GetUdpHeader ();
  
  if ((h) && (h->GetDestinationPort () >= m_minport) && (h->GetDestinationPort () <= m_maxport))
    {
      return true;
    }
  
  return false;
}


TcpSourcePortFec::TcpSourcePortFec(uint16_t port)
  : m_port (port)
{}

bool
TcpSourcePortFec::operator()(const CommonPacketData& pd) const
{
  TcpHeader* h = pd.GetTcpHeader ();
  
  if ((h) && (h->GetSourcePort () == m_port))
    {
      return true;
    }
  
  return false;
}



TcpSourcePortRangeFec::TcpSourcePortRangeFec(uint16_t minport, uint16_t maxport)
  : m_minport (minport),
    m_maxport (maxport)
{}

bool
TcpSourcePortRangeFec::operator()(const CommonPacketData& pd) const
{
  TcpHeader* h = pd.GetTcpHeader ();
  
  if ((h) && (h->GetSourcePort () >= m_minport) && (h->GetSourcePort () <= m_maxport))
    {
      return true;
    }
  
  return false;
}


TcpDestinationPortFec::TcpDestinationPortFec(uint16_t port)
  : m_port (port)
{}

bool
TcpDestinationPortFec::operator()(const CommonPacketData& pd) const
{
  TcpHeader* h = pd.GetTcpHeader ();
  
  if ((h) && (h->GetDestinationPort () == m_port))
    {
      return true;
    }
  
  return false;
}



TcpDestinationPortRangeFec::TcpDestinationPortRangeFec(uint16_t minport, uint16_t maxport)
  : m_minport (minport),
    m_maxport (maxport)
{}

bool
TcpDestinationPortRangeFec::operator()(const CommonPacketData& pd) const
{
  TcpHeader* h = pd.GetTcpHeader ();
  
  if ((h) && (h->GetDestinationPort () >= m_minport) && (h->GetDestinationPort () <= m_maxport))
    {
      return true;
    }
  
  return false;
}

} // namespace mpls
} // namespace ns3
