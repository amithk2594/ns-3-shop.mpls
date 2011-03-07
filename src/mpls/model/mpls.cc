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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/callback.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/loopback-net-device.h"

NS_LOG_COMPONENT_DEFINE ("Mpls");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (Mpls);

const uint16_t Mpls::PROT_NUMBER = 0x8847;

TypeId
Mpls::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Mpls")
    .SetParent<Object> ()
    .AddConstructor<Mpls> ()
  ;
  return tid;
}

Mpls::Mpls ()
  : m_node (0)
{
  NS_LOG_FUNCTION (this);
}

Mpls::~Mpls ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Mpls::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      if (node != 0)
        {
          m_node = node;
        }
    }

  Object::NotifyNewAggregate ();
}

void
Mpls::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (MplsInterfaceVector::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
    {
      *i = 0;
    }

  m_interfaces.clear ();

  m_node = 0;
  m_fib = 0;

  Object::DoDispose ();
}

uint32_t
Mpls::AddInterface (const Ptr<NetDevice> &device)
{
  NS_LOG_FUNCTION (this << &device);

  m_node->RegisterProtocolHandler (MakeCallback (&Mpls::Receive, this), PROT_NUMBER, device);

  Ptr<MplsInterface> interface = CreateObject<MplsInterface> ();
  interface->SetNode (m_node);
  interface->SetDevice (device);
  m_interfaces.push_back (interface);
  return m_interfaces.size () - 1;
}

Ptr<MplsInterface>
Mpls::GetInterfaceForDevice (const Ptr<NetDevice> &device) const
{
  NS_LOG_FUNCTION (this << &device);

  for (InterfaceVector::const_iterator i = m_interfaces.begin(); i != m_interfaces.end(); ++i)
    {
      if ((*i)->m_device == device)
        {
          return *i;
        }
    }
  return 0;
}

Ptr<MplsInterface>
Mpls::GetInterface (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);

  if (index < m_interfaces.size ())
    {
      return m_interfaces[index];
    }

  return 0;
}

uint32_t
Mpls::GetNInterfaces (void) const
{
  NS_LOG_FUNCTION_NOARGS ();

  return m_interfaces.size ();
}

const Ptr<MplsFib>&
Mpls::GetFib (void) const
{
  NS_LOG_FUNCTION_NOARGS ();

  return m_fib;
}

void
Mpls::Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from, const Address &to,
  NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  switch (packetType)
  {
    case NetDevice::PACKET_BROADCAST:
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Receive (): "
                    "dropping received packet -- broadcast packet type");
      return;
    case NetDevice::PACKET_MULTICAST:
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Receive (): "
                    "dropping received packet -- multicast packet type");
      return;
    default:
      break;
  }

  Ptr<Packet> packet = p->Copy ();
  MplsLabelStack stack;
  packet->RemoveHeader (stack);

  uint8_t ttl = stack.GetTop ().GetTtl ();
  if (ttl <= 1)
    {
      /* TTL exceeded */
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Receive (): "
                    "dropping received packet -- TTL exceeded");
      return;
    }

  MplsLabelStack stack;
  if (!nhlfe->GetOp ().execute (stack))
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteInput (): "
                    "dropping received packet -- invalid label operation");
      return DROP_PACKET;
    }

  stack.GetTop ().SetTtl (--ttl);
//  Ptr<NetDevice> outDev = RouteMpls (packet, stack, device->GetIfIndex ());
//  if (outDev == 0)
//    {
//      return;
//    }

  MplsForward (packet, stack, 0, outDev);
}

enum Mpls::ForwardingStatus
Mpls::RouteInput (const Ptr<const Packet> &p, const Ipv4Header &header, const Ptr<const NetDevice> &idev) const
{
  NS_LOG_FUNCTION (this << &idev << p << header);

  Ptr<MplsInterface> interface = GetInterfaceForDevice (idev);

  if (interface == 0)
    {
      // no mpls interface for device, IP forwarding
      return IP_FORWARD;
    }

  if (!m_ipv4Enabled)
    {
      if (m_strict)
        {
          return DROP_PACKET;
        }
      else
        {
          return IP_FORWARD;
        }
    }

  Ptr<MplsNhlfe> nhlfe = interface->GetFib ()->GetNhlfe (p, header);
  if (nhlfe == 0)
    {
      nhlfe = m_fib->GetNhlfe (p, header);
    }

  if (nhlfe == 0)
    {
      if (m_strict)
        {
          NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteInput (): "
                        "dropping received packet -- FTN not found -- "
                        "src=" << header.GetSource () << ", dst=" << header.GetDestination ());
          return DROP_PACKET;
        }
      else
        {
          return IP_FORWARD;
        }
    }

  /* check and decrement TTL */
  uint8_t ttl = header.GetTtl ();
  if (ttl <= 1)
    {
      /* TTL exceeded */
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteInput (): "
                    "dropping received packet -- TTL exceeded");
      return DROP_PACKET;
    }

  MplsLabelStack stack;

  Ptr<MplsInterface> interface = RouteMpls (nhlfe, p, stack);

  if (m_ipv6Enabled)
    {
      stack.PushFront (MplsStackEntry (MplsLabel::GetIpv4ExplicitNull ()));
    }
    
  if (interface != 0)
    {
      interface->Send (p, stack, header);
    }

//  if (!nhlfe->GetOp ().execute (stack))
//    {
//      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteInput (): "
//                    "dropping received packet -- invalid label operation");
//      return DROP_PACKET;
//    }

//  RouteMpls (

//  nhlfe->GetInterface ()->Send (p, header, stack);

  return MPLS_FORWARDED;
}

bool
Mpls::RouteMpls (const Ptr<MplsNhlfe> &nhlfe, Ptr<Packet> packet, MplsLabelStack &stack) const
{
  NS_LOG_FUNCTION (this);

  const MplsStackEntry& labelEntry = stack.GetTop ();

  for (MplsOp::Iterator i = nhlfe->GetOp ().Begin (), end = nhlfe->GetOp ().End (); i != end; ++i)
    {
      (*i)->execute (stack);
      uint32_t label = labelEntry->GetLabel ();

      if (label < MplsLabelStackEntry::MPLS_LABEL_RESERVED_MAX)
        {
          stack.Pop ();
          bool isBos = labelEntry->IsBos ();
          labelEntry = stack.GetTopEntry ();

          switch (label)
            {
            case MplsLabelStackEntry::MPLS_LABEL_IPV4NULL:
              NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                            "processing IPv4 Explicit NULL Label");

              if (isBos)
                {
                  IpForward (packet, 0, ttl);
                  return 0;
                }
              else
                {
                  continue;
                }

            case MplsLabelStackEntry::MPLS_LABEL_IPV6NULL:
              NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                            "processing IPv6 Explicit NULL Label");
              if (isBos)
                {
                  Ip6Forward (packet, 0, ttl);
                  return 0;
                }
              else
                {
                  continue;
                }
            }
          NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                        "unhandled reserved label");
        }

      libEntry = GetLibEntry (inIfIndex, label);

      if (libEntry == 0)
        {
          NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                        "dropping received packet -- no FLIB entry for label " << label);
          return 0;
        }

      switch (libEntry->GetOperation ())
        {
        case MplsLibEntry::SWAP:
          labelEntry = stack.Swap (libEntry->GetOutLabel ());
          break;

        case MplsLibEntry::PUSH:
          labelEntry = stack.Push (libEntry->GetOutLabel ());
          break;

        case MplsLibEntry::POP:
          labelEntry = stack.Pop ();

          if (labelEntry && labelEntry->IsBos ())
            {
              // XXX: add label AF lookup logic, now send via IPv4
              Ptr<NetDevice> outDev = GetDevice (libEntry->GetOutIfIndex ());
              IpForward (packet, outDev, ttl);
              return 0;
            }
          break;
        }

      if (labelEntry == 0)
        {
          NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                        "dropping received packet -- label operation under empty stack");
          return 0;
        }

      if (libEntry->GetOutIfIndex () >= 0)
        {
          break;
        }

      labelEntry = stack.GetTopEntry ();
      inIfIndex = -1;
    }

  if (i >= MAX_STACK_LOOPS)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                    "dropping received packet -- label operation limit exceeded");
      return 0;
    }

  Ptr<NetDevice> outDev = GetMplsDevice (libEntry->GetOutIfIndex ());
  if (outDev == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::RouteMpls (): "
                    "dropping received packet -- could not send packet through interface " << libEntry->GetOutIfIndex ());
      return 0;
    }

  labelEntry->SetTtl (ttl);
  return outDev;
}

enum Mpls::ForwardingStatus
Mpls::RouteInput (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev) const
{
  return MPLS_FORWARDED;
}

void
Mpls::IpForward (Ptr<Packet> packet, Ptr<NetDevice> outDev, uint8_t ttl) const
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::IpForward (): "
                "leaving mpls, send packet using Ipv4");

  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  if (ipv4 == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::IpForward (): "
                    "dropping received packet -- no Ipv4 Protocol");
      return;
    }

  if (ipv4->GetRoutingProtocol () == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::IpForward (): "
                    "dropping received packet -- no Ipv4 Routing Protocol");
      return;
    }

  Ipv4Header header;
  packet->RemoveHeader (header);
  header.SetTtl (ttl);
  Ipv4Address daddr = header.GetDestination ();
  Ipv4Address saddr = header.GetSource ();
  uint8_t protocol = header.GetProtocol ();
  Socket::SocketErrno errno_;

  Ptr<Ipv4Route> route = ipv4->GetRoutingProtocol ()->RouteOutput (packet, header, outDev, errno_);
  ipv4->Send (packet, saddr, daddr, protocol, route);
}

void
Mpls::Ip6Forward (Ptr<Packet> packet, Ptr<NetDevice> outDev, uint8_t ttl) const
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Ip6Forward (): "
                "leaving mpls, send packet using Ipv6");

  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();

  if (ipv6 == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Ip6Forward (): "
                    "dropping received packet -- no Ipv6 Protocol");
      return;
    }

  if (ipv6->GetRoutingProtocol () == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::Ip6Forward (): "
                    "dropping received packet -- no Ipv6 Routing Protocol");
    }

  Ipv6Header header;
  packet->RemoveHeader (header);
  header.SetHopLimit (ttl);
  Ipv6Address daddr = header.GetDestinationAddress ();
  Ipv6Address saddr = header.GetSourceAddress ();
  uint8_t nextHeader = header.GetNextHeader ();
  Socket::SocketErrno errno_;

  Ptr<Ipv6Route> route = ipv6->GetRoutingProtocol ()->RouteOutput (packet, header, outDev, errno_);
  ipv6->Send (packet, saddr, daddr, nextHeader, route);
}

void
Mpls::MplsForward (Ptr<Packet> packet, const MplsLabelStack &stack, const Header* ipHeader,
                                  Ptr<NetDevice> outDev) const
{
  NS_LOG_FUNCTION (this << packet << stack << &outDev);

  NS_ASSERT_MSG (stack.GetNEntries (), "Mpls::MplsForward (): "
                 "Fatal error, stack is empty");
  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::MplsForward (): "
                "send packet through interface " << outDev->GetIfIndex ());

  /* restore IpHeader */
  if (ipHeader != 0)
    {
      packet->AddHeader (*ipHeader);
    }

  /* place MPLS shim */
  packet->AddHeader (stack);

  if (packet->GetSize () > outDev->GetMtu ())
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::Mpls::MplsForward (): "
                    "dropping received packet -- MTU size exceeded");
      // XXX: need MTU Path Discover algoritm
      return;
    }

  // XXX: now only PointToPoint devices supported
  outDev->Send (packet, outDev->GetBroadcast (), PROT_NUMBER);
}

} // namespace mpls
} // namespace ns3
