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

#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "mpls-fec.h"

namespace ns3 {
namespace mpls {

Fec::~Fec ()
{
}

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


Ipv4SourceAddressPrefix::Ipv4SourceAddressPrefix (const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv4SourceAddressPrefix::Ipv4SourceAddressPrefix (char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4SourceAddressPrefix::operator() (PacketDemux &pd) const
{
  const Ipv4Header* h = pd.GetIpv4Header ();

  if (h && (m_mask.IsMatch (h->GetSource (), m_address)))
    {
      return true;
    }

  return false;
}


Ipv4DestinationAddressPrefix::Ipv4DestinationAddressPrefix (const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv4DestinationAddressPrefix::Ipv4DestinationAddressPrefix (char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4DestinationAddressPrefix::operator() (PacketDemux &pd) const
{
  const Ipv4Header* h = pd.GetIpv4Header ();

  if (h && (m_mask.IsMatch (h->GetDestination (), m_address)))
    {
      return true;
    }

  return false;
}


Ipv6SourceAddressPrefix::Ipv6SourceAddressPrefix (const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv6SourceAddressPrefix::Ipv6SourceAddressPrefix (char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6SourceAddressPrefix::operator() (PacketDemux &pd) const
{
  const Ipv6Header* h = pd.GetIpv6Header ();

  if (h && (m_mask.IsMatch (h->GetSourceAddress (), m_address)))
    {
      return true;
    }

  return false;
}

Ipv6DestinationAddressPrefix::Ipv6DestinationAddressPrefix (const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv6DestinationAddressPrefix::Ipv6DestinationAddressPrefix (char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6DestinationAddressPrefix::operator() (PacketDemux &pd) const
{
  const Ipv6Header* h = pd.GetIpv6Header ();

  if (h && (m_mask.IsMatch (h->GetDestinationAddress (), m_address)))
    {
      return true;
    }

  return false;
}


UdpSourcePort::UdpSourcePort (uint16_t port)
  : m_port (port)
{
}

bool
UdpSourcePort::operator() (PacketDemux& pd) const
{
  const UdpHeader* h = pd.GetUdpHeader ();

  if ((h) && (h->GetSourcePort () == m_port))
    {
      return true;
    }

  return false;
}

UdpSourcePortRange::UdpSourcePortRange (uint16_t minPort, uint16_t maxPort)
  : m_minPort (minPort),
    m_maxPort (maxPort)
{
}

bool
UdpSourcePortRange::operator() (PacketDemux& pd) const
{
  const UdpHeader* h = pd.GetUdpHeader ();

  if ((h) && (h->GetSourcePort () >= m_minPort) && (h->GetSourcePort () <= m_maxPort))
    {
      return true;
    }

  return false;
}


UdpDestinationPort::UdpDestinationPort (uint16_t port)
  : m_port (port)
{
}

bool
UdpDestinationPort::operator() (PacketDemux& pd) const
{
  const UdpHeader* h = pd.GetUdpHeader ();

  if (h && (h->GetDestinationPort () == m_port))
    {
      return true;
    }

  return false;
}



UdpDestinationPortRange::UdpDestinationPortRange (uint16_t minPort, uint16_t maxPort)
  : m_minPort (minPort),
    m_maxPort (maxPort)
{
}

bool
UdpDestinationPortRange::operator() (PacketDemux& pd) const
{
  const UdpHeader* h = pd.GetUdpHeader ();

  if (h && (h->GetDestinationPort () >= m_minPort) && (h->GetDestinationPort () <= m_maxPort))
    {
      return true;
    }

  return false;
}


TcpSourcePort::TcpSourcePort (uint16_t port)
  : m_port (port)
{
}

bool
TcpSourcePort::operator() (PacketDemux& pd) const
{
  const TcpHeader* h = pd.GetTcpHeader ();

  if (h && (h->GetSourcePort () == m_port))
    {
      return true;
    }

  return false;
}



TcpSourcePortRange::TcpSourcePortRange (uint16_t minPort, uint16_t maxPort)
  : m_minPort (minPort),
    m_maxPort (maxPort)
{
}

bool
TcpSourcePortRange::operator() (PacketDemux& pd) const
{
  const TcpHeader* h = pd.GetTcpHeader ();

  if (h && (h->GetSourcePort () >= m_minPort) && (h->GetSourcePort () <= m_maxPort))
    {
      return true;
    }

  return false;
}


TcpDestinationPort::TcpDestinationPort (uint16_t port)
  : m_port (port)
{
}

bool
TcpDestinationPort::operator() (PacketDemux& pd) const
{
  const TcpHeader* h = pd.GetTcpHeader ();

  if (h && (h->GetDestinationPort () == m_port))
    {
      return true;
    }

  return false;
}



TcpDestinationPortRange::TcpDestinationPortRange (uint16_t minPort, uint16_t maxPort)
  : m_minPort (minPort),
    m_maxPort (maxPort)
{
}

bool
TcpDestinationPortRange::operator() (PacketDemux& pd) const
{
  const TcpHeader* h = pd.GetTcpHeader ();

  if (h && (h->GetDestinationPort () >= m_minPort) && (h->GetDestinationPort () <= m_maxPort))
    {
      return true;
    }

  return false;
}

} // namespace mpls
} // namespace ns3
