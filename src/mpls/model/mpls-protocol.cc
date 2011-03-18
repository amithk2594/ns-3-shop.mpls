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
  
#define NS_LOG_APPEND_CONTEXT \
    if (m_node != 0) { \
      std::clog << Simulator::Now ().GetSeconds () \
      << " [node " << m_node->GetId () << "] "; }

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/callback.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "mpls-protocol.h"

NS_LOG_COMPONENT_DEFINE ("mpls::MplsProtocol");

namespace ns3 {
namespace mpls {

NS_OBJECT_ENSURE_REGISTERED (MplsProtocol);

TypeId
MplsProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsProtocol")
    .SetParent<Mpls> ()
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
    m_ilmTable (Create<IlmTable> ()),
    m_ftnTable (Create<FtnTable> ())
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
          m_ipv4 = DynamicCast<mpls::Ipv4Protocol> (ipv4);
          NS_ASSERT_MSG (m_ipv4 != 0, "Use mpls::Ipv4Protocol instead of default Ipv4");
        }
    }

  Object::NotifyNewAggregate ();
}

void
MplsProtocol::NotifyNewInterface (const Ptr<NetDevice> &device)
{
  NS_LOG_FUNCTION (this << &device);

  if (GetInterfaceForDevice (device) == 0)
    {
      AddInterface (device);
    }
}

void
MplsProtocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  for (InterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
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

int32_t
MplsProtocol::AddInterface (const Ptr<NetDevice> &device)
{
  NS_LOG_FUNCTION (this << &device);

  m_node->RegisterProtocolHandler (MakeCallback (&MplsProtocol::ReceiveMpls, this), Mpls::PROT_NUMBER, device);

  int32_t index = m_interfaces.size ();
  Ptr<Interface> interface = CreateObject<Interface> (index);
  interface->SetNode (m_node);
  interface->SetDevice (device);

  m_interfaces.push_back (interface);

  return index;
}

Ptr<Interface>
MplsProtocol::GetInterface (int32_t index) const
{
  if (index >= 0 && index < (int)m_interfaces.size ())
    {
      return m_interfaces[index];
    }
  return 0;
}

Ptr<Interface>
MplsProtocol::GetInterfaceForDevice (const Ptr<const NetDevice> &device) const
{
  for (InterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
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
  NS_LOG_FUNCTION (this << address);
  
  if (Ipv4Address::IsMatchingType (address))
    {
      if (m_ipv4 == 0)
        {
          NS_LOG_WARN ("Could not lookup next-hop --- ipv4 is not installed");
          return 0;
        }

      static Ipv4Header header;
      Socket::SocketErrno sockerr;

      header.SetDestination(Ipv4Address::ConvertFrom (address));

      Ptr<Ipv4RoutingProtocol> routing = m_ipv4->GetRoutingProtocol ();
      NS_ASSERT_MSG (routing != 0, "Need a ipv4 routing protocol object");

      // XXX: i don't know if we can do this
      Ptr<Ipv4Route> route = routing->RouteOutput (0, header, 0, sockerr);

      if (route != 0 && route->GetDestination ().Get () != 0)
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

bool
MplsProtocol::ReceiveIpv4 (const Ptr<Packet> &packet, const Ipv4Header &header, const Ptr<const NetDevice> &device)
{
  NS_LOG_DEBUG ("Classification of the received packet (idev " << device->GetIfIndex () << " " << header << ")");

  uint8_t ttl = header.GetTtl ();
  
  if (ttl <= 1)
    {
      NS_LOG_WARN ("Dropping received packet -- TTL exceeded");
      //m_dropTrace (...);
      return true;
    }

  m_demux.Assign (packet, header);
      
  Ptr<FecToNhlfe> ftn = LookupFtn (m_demux);
  
  m_demux.Release ();

  if (ftn == 0)
    {
      NS_LOG_DEBUG ("Dropping received packet -- ftn not found");
      //m_dropTrace (...)
      return false;
    }

  NS_LOG_DEBUG ("Found suitable entry -- " << Ptr<ForwardingInformation> (ftn) << 
                " with " << ftn->GetNNhlfe () << " available nhlfe");

  // push back ipv4 header
  packet->AddHeader (header);

  LabelStack stack;
  MplsForward (packet, ftn, stack, ttl - 1);

  return true;
}

bool
MplsProtocol::ReceiveIpv6 (const Ptr<Packet> &packet, const Ipv6Header &header, const Ptr<const NetDevice> &device)
{
  NS_ASSERT_MSG (0, "Ipv6 is not supported");
  return false;
}

void
MplsProtocol::ReceiveMpls (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol << from);

  NS_LOG_DEBUG ("Packet from " << from << " received on node " << m_node->GetId ());

  switch (packetType)
    {
      case NetDevice::PACKET_BROADCAST:
        NS_LOG_DEBUG ("Dropping received packet -- broadcast");
        //m_dropTrace (...);
        return;

      case NetDevice::PACKET_MULTICAST:
        NS_LOG_DEBUG ("Dropping received packet -- multicast");
        //m_dropTrace (...);
        return;

      default:
        break;
    }

  Ptr<Interface> interface = GetInterfaceForDevice (device);

  if (interface->IsUp ())
    {
      //m_rxTrace (...);
    }
  else
    {
      NS_LOG_DEBUG ("Dropping received packet -- interface " << interface->GetIfIndex () << " is disabled");
      //m_dropTrace (...);
      return;
    }

  Ptr<Packet> packet = p->Copy ();
  LabelStack stack;
  packet->RemoveHeader (stack);

  uint32_t sh = stack.Peek ();
  uint8_t ttl = shim::GetTtl (sh);

  NS_LOG_DEBUG ("Stack top label:" << (Label)shim::GetLabel (sh) << " ttl:" << (uint32_t)ttl );
  
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
      if (label == Label::IPV4_EXPLICIT_NULL)
        {
          if (!shim::IsBos (label))
            {
              NS_LOG_WARN ("Dropping received packet -- illegal Ipv4 explicit null label");
              // m_dropTrace
              return;
            }
          NS_LOG_DEBUG ("Ipv4_excplicit_null label was encountered -- ipv4 based forwarding must be used");
          IpForward (packet, ttl, 0, 0);
          return;
        }
      else if (label == Label::IPV6_EXPLICIT_NULL)
        {
          if (!shim::IsBos (label))
            {
              NS_LOG_WARN ("Dropping received packet -- illegal Ipv6 explicit null label");
              // m_dropTrace
              return;
            }
          // for future research
          NS_LOG_DEBUG ("Dropping received packet -- ipv6 is not supported");
          //m_dropTrace (...);
          return;
        }
      else if (label == Label::ROUTE_ALERT)
        {
          NS_LOG_WARN ("Skip label -- route alert label not supported");
        }
      else
        {
          NS_LOG_WARN ("Skip label -- unknown reserved label");
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

  NS_LOG_DEBUG ("Searching of label mapping for label " << (Label)label << 
                " if" << interface->GetIfIndex () << " dev" << device->GetIfIndex ());
  
  Ptr<IncomingLabelMap> ilm = LookupIlm (label, interface->GetIfIndex ());

  if (ilm == 0)
    {
      NS_LOG_DEBUG ("Dropping received packet -- ILM not found");
      //m_dropTrace (...);
      return;
    }

  NS_LOG_DEBUG ("Found suitable entry -- " << Ptr<ForwardingInformation> (ilm) << 
                " with " << ilm->GetNNhlfe () << " available nhlfe");

  MplsForward (packet, ilm, stack, ttl);
}

void
MplsProtocol::MplsForward (const Ptr<Packet> &packet, const Ptr<ForwardingInformation> &fwd, LabelStack &stack, 
    int8_t ttl)
{
  NS_LOG_FUNCTION (this << packet << fwd << stack << (uint32_t)ttl);

  Ptr<Interface> outInterface;
  Ptr<Ipv4Route> route;

  uint32_t stackSize = stack.GetSize ();

  NS_LOG_DEBUG ("Search of the suitable nhlfe for " << fwd);
  
  uint32_t idx = 0;
  // find first suitable nhlfe
  for (ForwardingInformation::Iterator i = fwd->Begin (); 
       i != fwd->End (); 
       ++i, ++idx)
    {
      const Nhlfe& nhlfe = *i;

      uint32_t opCode = nhlfe.GetOpCode ();
      int32_t outIfIndex = nhlfe.GetInterface ();      
      
      if (stackSize == 0 && opCode == OP_POP)
        {
          NS_LOG_WARN ("nhlfe " << idx << " " << nhlfe << " -- invalid nhlfe");
          continue;
        }
      
      // Perform ip forwarding if stack has only one label and 
      // nhlfe operation is POP
      if (outIfIndex < 0 && stackSize == 1 && nhlfe.GetOpCode () == OP_POP)
        {
          const Address &nextHop = nhlfe.GetNextHop ();
          
          if (!nextHop.IsInvalid ())
            {
              // next hop was specified
              route = GetNextHopRoute (nextHop);
              if (route == 0)
                {
                  NS_LOG_WARN ("nhlfe " << idx << " " << nhlfe << " -- next-hop is unavailable");
                  continue;
                }
            }
          else
            {
              route = 0;
            }
          NS_LOG_DEBUG ("nhlfe " << idx << " " << nhlfe << " selected (*)");
          NS_LOG_DEBUG ("Stack is empty -- ipv4 based forwarding must be used");
          IpForward (packet, ttl, 0, route);
          return;
        }

      if (outIfIndex >= 0)
        {
          route = 0;
          outInterface = GetInterface (outIfIndex);
          if (outInterface == 0)
            {
              NS_LOG_WARN ("nhlfe " << idx << " " << nhlfe << " -- invalid outgoing interface");
              continue;
            }
        }
      else
        {
          outInterface = 0;
          const Address &nextHop = nhlfe.GetNextHop ();
          
          route = GetNextHopRoute (nextHop);
          if (route != 0)
            {
              NS_LOG_LOGIC ("Next-hop route dest:" << route->GetDestination () << " "
                 << "src: " << route->GetSource () << " "
                 << "gw: " << route->GetGateway ());

              outInterface = GetInterfaceForDevice (route->GetOutputDevice ());
            }

          if (outInterface == 0)
            {
              NS_LOG_WARN ("nhlfe " << idx << " " << nhlfe << " -- next-hop is unavailable");
              continue;
            }
        }

      if (outInterface->IsUp ())
        {
          NS_LOG_DEBUG ("nhlfe " << idx << " " << nhlfe << " selected (*)");

          if (!RealMplsForward (packet, nhlfe, stack, ttl, outInterface))
            {
              IpForward (packet, ttl, outInterface->GetDevice (), route);
            }
          return;
        }
      else
        {
          NS_LOG_DEBUG ("nhlfe " << idx << " " << nhlfe << " -- mpls interface disabled");
        }
    }

  NS_LOG_DEBUG ("Dropping received packet -- there is no suitable nhlfe");
  //m_dropTrace (...);
}

Ptr<IncomingLabelMap>
MplsProtocol::LookupIlm (Label label, int32_t interface)
{
  NS_LOG_FUNCTION (this << label << interface);

  NS_ASSERT_MSG (m_ilmTable != 0, "IlmTable is not installed");

  IlmTable::Iterator begin = m_ilmTable->Begin ();
  IlmTable::Iterator end = m_ilmTable->End ();
  Ptr<IncomingLabelMap> ilm;

  // TODO: Different search for both label spaces

  for (IlmTable::Iterator i = begin; i != end; ++i)
    {
      ilm = (*i).second;
      if (ilm->GetLabel () == label && ilm->GetInterface () == interface)
        {
          return ilm;
        }
    }

  for (IlmTable::Iterator i = begin; i != end; ++i)
    {
      ilm = (*i).second;
      if (ilm->GetLabel () == label && ilm->GetInterface () < 0)
        {
          return ilm;
        }
    }

  return 0;
}

Ptr<FecToNhlfe>
MplsProtocol::LookupFtn (PacketDemux &demux)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (m_ftnTable != 0, "FtnTable is not installed");

  FtnTable::Iterator begin = m_ftnTable->Begin ();
  FtnTable::Iterator end = m_ftnTable->End ();

  for (FtnTable::Iterator i = begin; i != end; ++i)
    {
      const Ptr<FecToNhlfe> &ftn = (*i).second;
      if ((ftn->GetFec ()) (demux))
        {
          return ftn;
        }
    }

  return 0;
}

bool
MplsProtocol::RealMplsForward (const Ptr<Packet> &packet, const Nhlfe &nhlfe, LabelStack &stack, int8_t ttl,
    const Ptr<Interface> &outInterface)
{
  NS_LOG_FUNCTION (this << packet << nhlfe << stack << (uint32_t)ttl << outInterface);

  switch (nhlfe.GetOpCode ())
    {
      case OP_POP:
        NS_ASSERT_MSG (!stack.IsEmpty (), "POP operation on the empty stack");
        stack.Pop ();
        break;

      case OP_SWAP:
        for (uint32_t i = 0, c = nhlfe.GetNLabels (); i < c; ++i)
          {
            Label label = nhlfe.GetLabel (i);
            if (label == Label::IMPLICIT_NULL)
              {
                // Penultimate Hop Popping
                NS_LOG_DEBUG ("Pop the stack, implicit_null label was encountered -- "
                              "ipv4 based forwarding must be used");
                return false;
              }
            if (i)
              {
                stack.Push (shim::Get (label));
              }
            else  
              {
                stack.Swap (shim::Get (label));
              }
          }
        break;

      default:
        NS_ASSERT_MSG (0, "Invalid operation code");
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

  NS_LOG_DEBUG ("Sendind labeled packet via if" << outInterface->GetIfIndex () << " " << 
                "dev" << outInterface->GetDevice ()-> GetIfIndex ());

  if (!outInterface->Send (packet))
    {
      // m_dropTrace
    }
  else
    {
      // m_txTrace
    }

  return true;
}

void
MplsProtocol::IpForward (const Ptr<Packet> &packet, uint8_t ttl, const Ptr<NetDevice> &outDev, 
  const Ptr<Ipv4Route> &route)
{
  NS_LOG_FUNCTION (this << packet << (uint32_t)ttl << outDev << route);

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
      NS_ASSERT_MSG (routing != 0, "Need a ipv4 routing protocol object");

      m_ipv4->SendWithHeader (packet, header, routing->RouteOutput (packet, header, outDev, sockerr));
    }
  else
    {
      m_ipv4->SendWithHeader (packet, header, route);
    }
}

//NS_OBJECT_ENSURE_REGISTERED (IpProbe);

TypeId
IpProbe::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::IpProbe")
    .SetParent<Header> ()
    .AddConstructor<IpProbe> ()
  ;
  return tid;
}

TypeId
IpProbe::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

IpProbe::IpProbe()
{
  m_version = 0;
}

IpProbe::~IpProbe()
{
}

uint8_t
IpProbe::GetIpVersion (const Ptr<Packet> &packet)
{
  IpProbe ip;
  
  packet->PeekHeader (ip);
    
//  if (ip.m_version != 4 && ip.m_version != 6)
//    NS_LOG_DEBUG ("Returning undefined IP protocol version");

  return ip.m_version;
}

uint32_t
IpProbe::GetSerializedSize (void) const
{
//  NS_LOG_WARN ("Returning invalid serialized size");
  
  return 0;
}

void IpProbe::Serialize (Buffer::Iterator start) const
{
//  NS_LOG_WARN ("Doing nothing");
}

uint32_t
IpProbe::Deserialize (Buffer::Iterator start)
{
  m_version = start.ReadU8 () >> 4;

  return 0;
}

void
IpProbe::Print (std::ostream& os) const
{
}


} // namespace mpls
} // namespace ns3
