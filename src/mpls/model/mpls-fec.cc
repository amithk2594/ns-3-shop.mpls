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


Ipv4Source::Ipv4Source (const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv4Source::Ipv4Source (char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4Source::operator() (PacketDemux &pd) const
{
  const Ipv4Header* h = pd.GetIpv4Header ();

  if (h && (m_mask.IsMatch (h->GetSource (), m_address)))
    {
      return true;
    }

  return false;
}

void
Ipv4Source::Print (std::ostream &os) const
{
  os << "inet src:" << m_address;
  if (m_mask.GetPrefixLength ())
    {
      os << "/" << m_mask.GetPrefixLength ();
    }
}

Ipv4Destination::Ipv4Destination (const Ipv4Address &address, const Ipv4Mask &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv4Destination::Ipv4Destination (char const *address)
  : m_mask (Ipv4Mask ("/32"))
{
  AsciiToPrefix (address, m_address, m_mask, true);
}

bool
Ipv4Destination::operator() (PacketDemux &pd) const
{
  const Ipv4Header* h = pd.GetIpv4Header ();

  if (h && (m_mask.IsMatch (h->GetDestination (), m_address)))
    {
      return true;
    }

  return false;
}

void
Ipv4Destination::Print (std::ostream &os) const
{
  os << "inet dst:" << m_address;
  if (m_mask.GetPrefixLength ())
    {
      os << "/" << m_mask.GetPrefixLength ();
    }
}

Ipv6Source::Ipv6Source (const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv6Source::Ipv6Source (char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6Source::operator() (PacketDemux &pd) const
{
  const Ipv6Header* h = pd.GetIpv6Header ();

  if (h && (m_mask.IsMatch (h->GetSourceAddress (), m_address)))
    {
      return true;
    }

  return false;
}

void
Ipv6Source::Print (std::ostream &os) const
{
  os << "inet6 src:" << m_address;
  if (m_mask.GetPrefixLength ())
    {
      os << "/" << m_mask.GetPrefixLength ();
    }
}

Ipv6Destination::Ipv6Destination (const Ipv6Address &address, const Ipv6Prefix &mask)
  : m_address (address),
    m_mask (mask)
{
}

Ipv6Destination::Ipv6Destination (char const *address)
  : m_mask (Ipv6Prefix (128))
{
  AsciiToPrefix (address, m_address, m_mask, false);
}

bool
Ipv6Destination::operator() (PacketDemux &pd) const
{
  const Ipv6Header* h = pd.GetIpv6Header ();

  if (h && (m_mask.IsMatch (h->GetDestinationAddress (), m_address)))
    {
      return true;
    }

  return false;
}

void
Ipv6Destination::Print (std::ostream &os) const
{
  os << "inet6 dst:" << m_address;
  if (m_mask.GetPrefixLength ())
    {
      os << "/" << m_mask.GetPrefixLength ();
    }
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

void
UdpSourcePort::Print (std::ostream &os) const
{
  os << "udp src:" << m_port;
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

void
UdpSourcePortRange::Print (std::ostream &os) const
{
  os << "udp src:" << m_minPort << "-" << m_maxPort;
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

void
UdpDestinationPort::Print (std::ostream &os) const
{
  os << "udp dst:" << m_port;
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

void
UdpDestinationPortRange::Print (std::ostream &os) const
{
  os << "udp dst:" << m_minPort << "-" << m_maxPort;
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

void
TcpSourcePort::Print (std::ostream &os) const
{
  os << "tcp src:" << m_port;
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

void
TcpSourcePortRange::Print (std::ostream &os) const
{
  os << "tcp src:" << m_minPort << "-" << m_maxPort;
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

void
TcpDestinationPort::Print (std::ostream &os) const
{
  os << "tcp dst:" << m_port;
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

void
TcpDestinationPortRange::Print (std::ostream &os) const
{
  os << "tcp dst:" << m_minPort << "-" << m_maxPort;
}

} // namespace mpls
} // namespace ns3
