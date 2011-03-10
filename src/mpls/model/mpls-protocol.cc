/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010-2011 Andrey Churin, Stefano Avallone
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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/callback.h"

#include "mpls-protocol.h"

NS_LOG_COMPONENT_DEFINE ("MplsProtocol");

namespace ns3 {
namespace mpls {

const uint16_t MplsProtocol::PROT_NUMBER = 0x8847;

TypeId
MplsProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsProtocol")
    .SetParent<Object> ()
    .AddConstructor<MplsProtocol> ()
    .AddTraceSource ("Tx", "Send packet to outgoing interface.",
                        MakeTraceSourceAccessor (&MplsProtocol::m_txTrace))
    .AddTraceSource ("Rx", "Receive packet from incoming interface.",
                        MakeTraceSourceAccessor (&MplsProtocol::m_rxTrace))
    .AddTraceSource ("Drop", "Drop packet",
                        MakeTraceSourceAccessor (&MplsProtocol::m_dropTrace))
  ;
  return tid;
}

MplsProtocol::MplsProtocol ()
  : m_node (0),
    m_ipv4 (0),
    m_ilmTable (0)
{
  NS_LOG_FUNCTION (this);
}

MplsProtocol::~MplsProtocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MplsProtocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      
      if (node != 0)
        {
          m_node = node;
        }
    }
    
  if (m_ipv4 == 0)
    {
      Ptr<Ipv4> ipv4 = this->GetObject<Ipv4> ();
      if (ipv4 != 0)
        {
          m_ipv4 = DynamicCast<MplsIpv4Protocol> (ipv4);
          NS_ASSERT_MSG (m_ipv4, "Use MplsIpv4Protocol instead of default Ipv4");
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
  m_ipv4 = 0;
  m_ilmTable = 0;

  Object::DoDispose ();
}

void
MplsProtocol::SetIlmTable (const Ptr<IlmTable> &ilmTable)
{
  m_ilmTable = ilmTable;
}

Ptr<IlmTable>
MplsProtocol::GetIlmTable (void) const
{
  return m_ilmTable;
}

void
MplsProtocol::SetFtnTable (const Ptr<FtnTable> &ftnTable)
{
  m_ftnTable = ftnTable;
}

Ptr<FtnTable>
MplsProtocol::GetFtnTable (void) const
{
  return m_ftnTable;
}

uint32_t
MplsProtocol::AddInterface (const Ptr<NetDevice> &device)
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
MplsProtocol::GetInterfaceForDevice (const Ptr<NetDevice> &device) const
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

Ptr<Ipv4Route>
MplsProtocol::GetNextHopRoute (const Address &address) const
{
  if (Ipv4Address::IsMatchingType (address))
    {
      if (!m_ipv4)
        {
          NS_LOG_WARN ("Could not lookup next-hop --- ipv4 is not installed");
          return 0;
        }

      static Ipv4Header header;
      Socket::SocketErrno sockerr;

      header.SetDestination(Ipv4Address::ConvertFrom (address));
      
      Ptr<Ipv4RoutingProtocol> routing = m_ipv4->GetRoutingProtocol ();
      NS_ASSERT_MSG (routing, "Need a ipv4 routing protocol object");
      
      // XXX: i don't know if we can do this
      Ptr<Ipv4Route> route = routing->RouteOutput (0, header, 0, sockerr);

      if (route && route->GetDestination ().Get () != 0)
        {
          return route;
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

  NS_ASSERT_MSG (m_ilmTable != 0, "IlmTable is not installed");
  
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

  if (!mplsInterface->IsUp ()) 
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
  
  if (ttl <= 1)
    {
      NS_LOG_WARN ("Dropping received packet -- TTL exceeded");
      //m_dropTrace (...);
      return;
    }

  ttl--;

  uint32_t label = shim::GetLabel (sh);

  // process reserved labels
  while (label < 0x10)
    {
      switch (label) 
        {
          case Label::IPV4_EXPLICIT_NULL:
            if (!shim::IsBos (label))
              {
                NS_LOG_WARN ("Dropping received packet -- illegal Ipv4 explicit null label");
                // m_dropTrace 
                return;
              }
            NS_LOG_LOGIC ("Force Ipv4 forwarding");
            IpForward ();
            break;

          case Label::IPV6_EXPLICIT_NULL:
            if (!shim::IsBos (label))
              {
                NS_LOG_WARN ("Dropping received packet -- illegal Ipv6 explicit null label");
                // m_dropTrace 
                return;
              }
            // for future research
            NS_LOG_LOGIC ("Dropping received packet -- ipv6 is not supported"); 
            //m_dropTrace (...);
            return;
            
          case Label::ROUTE_ALERT:
            NS_LOG_WARN ("Skip label -- route alert label not supported");
            break;

          default:
            NS_LOG_WARN ("Skip label -- unknown reserved label");
            break;
        }

      stack.Pop ();
      
      if (stack.IsEmpty ()) break;
      
      sh = stack.Peek ();
      label = shim::GetLabel (sh);
    }

  if (stack.IsEmpty ()) {
    NS_LOG_WARN ("Dropping received packet -- empty label stack");
    //m_dropTrace (...);
    return;
  }
  
  Ptr<IncomingLabelMap> ilm = LookupIlm (label, mplsInterface->GetIndex ());
  
  if (ilm == 0)
    {
      NS_LOG_LOGIC ("Dropping received packet -- ILM not found");
      //m_dropTrace (...);
      return;
    }

  Ptr<MplsInterface> outInterface;
  Ptr<Ipv4Route> route;
  
  for (ForwardingInformation::Iterator i = ilm->Begin (); i != ilm->End (); ++i)
    {
      
      uint32_t outIfIndex = (*i)->GetInterface ();
      
      if (outIfIndex)
        {
          route = 0;
          outInterface = GetInterface (outIfIndex);
          if (!outInterface)
            {
              NS_LOG_WARN ("Invalid outgoing interface index " << outIfIndex);
            }
        }
      else
        {
          outInterface = 0;
          const Address &nextHop = (*i)->GetNextHop ();
          route = GetNextHopRoute (nextHop);
          if (route)
            {
              outInterface = GetInterfaceForDevice (route->GetDevice ());
            }

          if (!outInterface)
            {
              NS_LOG_WARN ("Invalid invalid next-hop address " << nextHop);
            }
        }

      if (outInterface && outInterface->IsUp ())
        {
          if (!RealMplsForward (packet, nhlfe, stack, ttl, outInterface))
            {
              IpForward (packet, ttl, outInterface->GetDevice (), route);
            }
          return;
        }
    }

  NS_LOG_LOGIC ("Dropping received packet -- there is no suitable NHLFE");
  //m_dropTrace (...);
}

Ptr<IncomingLabelMap>
MplsProtocol::LookupIlm (Label label, uint32_t interface)
{
  IlmTable::Iterator begin = m_ilmTable->Begin ();
  IlmTable::Iterator end = m_ilmTable->End ();
  Ptr<IncomingLabelMap> ilm;
  
  for (IlmTable::Iterator i = begin; i != end; ++i)
    {
      ilm = (*i)->second;
      if (ilm->GetLabel () == label && ilm->GetInterface () == interface)
        {
          return ilm;
        }
    }

  for (IlmTable::Iterator i = begin; i != end; ++i)
    {
      ilm = (*i)->second;
      if (ilm->GetLabel () == label && ilm->GetInterface () == 0)
        {
          return ilm;
        }
    }

  return 0;
}

void
MplsProtocol::MplsForward (Ptr<Packet> &packet, const Nhlfe &nhlfe, int8_t ttl, Ptr<MplsInterface> &outInterface)
{
  LabelStack stack;
  if (!RealMplsForward (packet, nhlfe, stack, ttl, outInterface))
    {
      // really no makes sense
      IpForward (packet, ttl, 0, 0);
    }
}

bool
MplsProtocol::RealMplsForward (Ptr<Packet> &packet, const Nhlfe &nhlfe, LabelStack &stack, int8_t ttl, 
    Ptr<MplsInterface> &outInterface)
{
  NS_LOG_FUNCTION (this);
  
  switch (nhlfe.m_opcode)
    {  
      case OP_POP:
        NS_ASSERT_MSG (!stack.IsEmpty (), "POP operation on the empty stack");
        stack.Pop ();
        break;
        
      case OP_SWAP:
        for (uint32_t i = 0, count = nhlfe.m_count; i < count; ++i)
          {
            label = nhlfe.m_labels[i];
            if (label == Label::IMPLICIT_NULL)
              {
                // Penultimate Hop Popping
                NS_LOG_LOGIC ("Penultimate Hop Popping");
                return false;
              }

            i ? stack.Push (label) : stack.Swap (label);
          }
        break;
        
      default:
        NS_ASSERT_MSG (0, "Invalid operation code");
    }

  if (stack.IsEmpty ())
    {
      return false;
    }

  // A labeled IP datagram whose size exceeds the Conventional Maximum
  // Frame Payload Size of the data link over which it is to be forwarded
  // MAY be considered to be "too big".

  // A labeled IP datagram whose size exceeds the True Maximum Frame
  // Payload Size of the data link over which it is to be forwarded MUST
  // be considered to be "too big".

  // A labeled IP datagram which is not "too big" MUST be transmitted
  // without fragmentation.
   
  shim::SetTtl (stack.Peek (), ttl);
  packet->AddHeader (stack);

  outInterface->Send (packet);
  //m_txTrace;
  
  return true;
}

void
MplsProtocol::IpForward (Ptr<Packet> &packet, uint8_t ttl, Ptr<NetDevice> &outDev, Ptr<Ipv4Route> route)
{
  if (m_ipv4 == 0)
    {
      NS_LOG_WARN ("Dropping received packet -- ipv4 is not installed");
      // m_dropTrace
      return;
    }

  Ipv4Header header;
  packet->RemoveHeader (header);
  header.SetTtl (ttl);
  
  // if there is no route
  if (route == 0)
    {
      Socket::SocketErrno sockerr;

      Ptr<Ipv4RoutingProtocol> routing = m_ipv4->GetRoutingProtocol ();
      NS_ASSERT_MSG (routing, "Need a ipv4 routing protocol object");
      route = routing->RouteOutput (packet, header, outDev, sockerr);
    }
    
  m_ipv4->SendWithHeader (packet, header, route);
}

} // namespace mpls
} // namespace ns3
