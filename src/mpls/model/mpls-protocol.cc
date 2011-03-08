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

#include "mpls-protocol.h"

NS_LOG_COMPONENT_DEFINE ("MplsProtocol");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (MplsProtocol);

const uint16_t MplsProtocol::PROT_NUMBER = 0x8847;

TypeId
MplsProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsProtocol")
    .SetParent<Object> ()
    .AddConstructor<MplsProtocol> ()
  ;
  return tid;
}

MplsProtocol::MplsProtocol ()
  : m_node (0)
{
  NS_LOG_FUNCTION (this);
}

MplsProtocol::~MplsProtocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MplsProtocol::SetNode (Ptr<Node> &node)
{
  m_node = node;
}

void
MplsProtocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      // verify that it's a valid node and that
      // the node has not been set before
      if (node != 0)
        {
          this->SetNode (node);
        }
    }

  Object::NotifyNewAggregate ();
}

void
MplsProtocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  for (MplsInterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
    {
      *i = 0;
    }
  m_interfaces.clear ();
  m_node = 0;

  Object::DoDispose ();
}

uint32_t
MplsProtocol::AddInterface (Ptr<NetDevice> &device)
{
  NS_LOG_FUNCTION (this << &device);
  
  m_node->RegisterProtocolHandler (MakeCallback (&MplsProtocol::Receive, this), PROT_NUMBER, device);
  
  Ptr<MplsInterface> interface = CreateObject<MplsInterface> ();
  interface->SetNode (m_node);
  interface->SetDevice (device);

  uint32_t index = m_interfaces.size ();
  m_interfaces.push_back (interface);
  return index;
}

Ptr<MplsInterface>
MplsProtocol::GetInterface (uint32_t index) const
{
  if (index < m_interfaces.size ())
    {
      return m_interfaces[index];
    }
  return 0;
}

Ptr<MplsInterface>
MplsProtocol::GetInterfaceForDevice (Ptr<const NetDevice> &device) const
{
  for (MplsInterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }

  return 0;
}

uint32_t 
MplsProtocol::GetNInterfaces (void) const
{
  return m_interfaces.size ();
}

void 
MplsProtocol::Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol << from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " << m_node->GetId ());
    
  switch (packetType)
    {
      case NetDevice::PACKET_BROADCAST:
        NS_LOG_LOGIC ("Dropping received packet -- broadcast");
        //m_dropTrace (...);
        return;

      case NetDevice::PACKET_MULTICAST:
        NS_LOG_LOGIC ("Dropping received packet -- multicast");
        //m_dropTrace (...);
        return;
        
      default:
        break;
    }

  Ptr<MplsInterface> mplsInterface = GetInterfaceForDevice (device);
  if (mplsInterface->IsUp ()) 
    {
      //m_rxTrace (...);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      //m_dropTrace (...);
      return;
    }
    
  Ptr<Packet> packet = p->Copy ();
  LabelStack stack;
  packet->RemoveHeader (stack);
  
  uint32_t sh = stack.Peek ();
  uint8_t ttl = shim::GetTtl (sh);
  
  if (!ProcessReservedLabel (stack))
    {
      return;
    }

  if (stack.IsEmpty ()) {
    NS_LOG_WARN ("Dropping received packet -- empty label stack");
    //m_dropTrace (...);
    return false;
  }
  
  if (ttl <= 1)
    {
      NS_LOG_WARN ("Dropping received packet -- TTL exceeded");
      //m_dropTrace (...);
      return;
    }

  // lookup nhlfe

  MplsForward (packet, nhlfe, stack, ttl);
}

bool
MplsProtocol::ProcessReservedLabel (LabelStack& stack)
{
  while (!stack.IsEmpty ())
    {
      sh = stack.Peek ();
      uint32_t label = shim::GetLabel (sh);

      if (label < 0x10)
        {
          stack.Pop ();
          if (label == Label::IPV4_EXPLICIT_NULL)
            {
              if (!shim::IsBos (label))
                {
                  NS_LOG_WARN ("Illegal Ipv4 explicit null label position");
                }
              // Ipv4 forward
            }
          else if (label == Label::IPV6_EXPLICIT_NULL)
            {
              // Ipv6 forward
              NS_LOG_LOGIC ("Dropping received packet -- ipv6 is not supported"); 
              //m_dropTrace (...);
              return false;
            }
          else
            {
              NS_LOG_WARN ("Skip reserved label -- unknown label");
              return false;
              // Invalid label, skip
            }
        }
      else 
        {
          break;
        }
    }

  return true;
}

void
MplsProtocol::MplsForward (Ptr<Packet> &packet, MplsLabelStack &stack, const Nhlfe& nhlfe, int8_t ttl)
{
  NS_LOG_FUNCTION (this);

  OperationIterator iterator (nhlfe.GetOperations ());
  
  Ptr<MplsInterface> outInterface = nhlfe.GetInterface ();  
  
  if (outInterface == 0)
    {
      NS_LOG_LOGIC ("Dropping received packet -- bad outgoing interface");
      //m_dropTrace (...);
    }
    
  while (iterator.HasNext ()) 
    {
      if (stack.IsEmpty ())
        {
          NS_LOG_WARN ("Operations has been performed partly");
          break;
        }
        
      uint32_t opcode = iterator.Get ();
      
      if (opcode == OP_PUSH)
        {
          uint32_t label = iterator.Get ();
          if (label == Label::IMPLICIT_NULL)
            {
              NS_LOG_LOGIC ("Penultimate Hop Popping");
              // Penultimate Hop Popping
              
              return;
            }

          stack.Push (label);
        }
      else if (opcode == OP_POP)
        {
          if (stack.IsEmpty ()) 
            {
              NS_LOG_WARN ("Dropping received packet -- POP over an empty stack");
              //m_dropTrace (...);
              return;
            }

          stack.Pop ();
          
          if (!ProcessReservedLabel ())
            {
              return;
            }
        }
      else if (opcode == OP_SWAP)
        {
          uint32_t label = iterator.Get ();
          if (label == Label::IMPLICIT_NULL)
            {
              // Penultimate Hop Popping
              return;
            }

          stack.Swap (label);
        }
      else
        {
          NS_ASSERT_MSG (0, "Invalid operation code");
        }
    }

  ttl--;
  
  if (!stack.IsEmpty ())
    {

      shim::SetTtl (stack.Peek (), ttl);
      packet->AddHeader (stack);
      // XXX: if interface is down? Possible nhlfe should not be selected if outgoing interface is down?
      outInterface->Send (packet);
    }
  else
    {
      IpForward (packet, interface ? interface->GetDevice () : 0, ttl);
    }
}

void
MplsProtocol::IpForward (Ptr<Packet> &packet, Ptr<NetDevice> outDev, uint8_t ttl) const
{
  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::IpForward (): "
                "leaving mpls, send packet using Ipv4");

  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  if (ipv4 == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::IpForward (): "
                    "dropping received packet -- no Ipv4 Protocol");
      return;
    }

  if (ipv4->GetRoutingProtocol () == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::IpForward (): "
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
MplsProtocol::Ip6Forward (Ptr<Packet> packet, Ptr<NetDevice> outDev, uint8_t ttl) const
{
  NS_LOG_FUNCTION (this << packet);

  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::Ip6Forward (): "
                "leaving mpls, send packet using Ipv6");

  Ptr<Ipv6L3Protocol> ipv6 = m_node->GetObject<Ipv6L3Protocol> ();

  if (ipv6 == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::Ip6Forward (): "
                    "dropping received packet -- no Ipv6 Protocol");
      return;
    }

  if (ipv6->GetRoutingProtocol () == 0)
    {
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::Ip6Forward (): "
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
MplsProtocol::MplsForward (Ptr<Packet> packet, const MplsLabelStack &stack, const Header* ipHeader,
                                  Ptr<NetDevice> outDev) const
{
  NS_LOG_FUNCTION (this << packet << stack << &outDev);

  NS_ASSERT_MSG (stack.GetNEntries (), "MplsProtocol::MplsForward (): "
                 "Fatal error, stack is empty");
  NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::MplsForward (): "
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
      NS_LOG_DEBUG ("Node[" << m_node->GetId () << "]::MplsProtocol::MplsForward (): "
                    "dropping received packet -- MTU size exceeded");
      // XXX: need MTU Path Discover algoritm
      return;
    }

  // XXX: now only PointToPoint devices supported
  outDev->Send (packet, outDev->GetBroadcast (), PROT_NUMBER);
}

void
MplsProtocol::AddLibEntry (Ptr<const MplsLibEntry> entry)
{
  m_entries.push_back (entry);
}

void
MplsProtocol::RemoveLibEntry (Ptr<const MplsLibEntry> entry)
{
  m_entries.remove (entry);
}

//void
//MplsProtocol::RemoveLibEntry (int32_t ifIndex, uint32_t label)
//{
//  MplsLibEntryList::iterator i = m_entries.begin ();
//  while (i != m_entries.end ())
//    {
//      if ((*i)->GetInIfIndex () == ifIndex && (*i)->GetLabel () == label)
//        {
//          i = m_entries.erase (i);
//        }
//      else
//        {
//          ++i;
//        }
//    }
//}

//void
//MplsProtocol::RemoveLibEntry (const ForwardEquivalenceClass &fec)
//{
//  MplsLibEntryList::iterator i = m_entries.begin ();
//  while (i != m_entries.end ())
//    {
//      if (*((*i)->GetFec ()) == fec)
//        {
//          i = m_entries.erase (i);
//        }
//      else
//        {
//          ++i;
//        }
//    }
//}

//void
//MplsProtocol::RemoveLibEntry (int32_t ifIndex)
//{
//  MplsLibEntryList::iterator i = m_entries.begin ();
//  while (i != m_entries.end ())
//    {
//      if ((*i)->GetInIfIndex () == ifIndex)
//        {
//          i = m_entries.erase (i);
//        }
//      else
//        {
//          ++i;
//        }
//    }
//}

Ptr<const MplsLibEntry>
MplsProtocol::GetLibEntry (int32_t ifIndex, uint32_t label) const
{
  NS_LOG_FUNCTION (this);

  for (MplsLibEntryList::const_iterator it = m_entries.begin (); it != m_entries.end (); ++it)
    {
      if ((*it)->GetInIfIndex () == ifIndex && (*it)->GetInLabel () == label)
        {
          return *it;
        }
    }

  return 0;
}

Ptr<const MplsLibEntry>
MplsProtocol::GetLibEntry (int32_t ifIndex, Ptr<const Packet> p, const Ipv4Header &header) const
{
  NS_LOG_FUNCTION (this);

  uint32_t match = -1;
  Ptr<const MplsLibEntry> entry = 0;

  for (MplsLibEntryList::const_iterator it = m_entries.begin (); it != m_entries.end (); ++it)
    {
      const ForwardingEquivalenceClass *fec = (*it)->GetFec ();
      if (fec != 0)
        {
          uint32_t t = fec->GetMatch (p, header);
          if (t < match)
            {
              match = t;
              entry = *it;
            }
        }
    }

  return entry;
}

Ptr<const MplsLibEntry>
MplsProtocol::GetLibEntry (int32_t ifIndex, Ptr<const Packet> p, const Ipv6Header &header) const
{
  NS_LOG_FUNCTION (this);

  uint32_t match = -1;
  Ptr<const MplsLibEntry> entry = 0;

  for (MplsLibEntryList::const_iterator it = m_entries.begin (); it != m_entries.end (); ++it)
    {
      const ForwardingEquivalenceClass *fec = (*it)->GetFec ();
      if (fec != 0)
        {
          uint32_t t = fec->GetMatch (p, header);
          if (t < match)
            {
              match = t;
              entry = *it;
            }
        }
    }

  return entry;
}

void
MplsProtocol::PrintLibEntries (std::ostream &os) const
{
  NS_LOG_FUNCTION (this);

  for (MplsLibEntryList::const_iterator it = m_entries.begin (); it != m_entries.end (); ++it)
    {
      (*it)->Print (os);
      os << std::endl;
    }
}

} // namespace mpls
} // namespace ns3
