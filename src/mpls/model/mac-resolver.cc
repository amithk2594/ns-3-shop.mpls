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

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/object-vector.h"
#include "ns3/trace-source-accessor.h"

#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "arp-header.h"
#include "arp-cache.h"
#include "ipv4-interface.h"

NS_LOG_COMPONENT_DEFINE ("MacResolver");

namespace ns3 {

const uint16_t MacResolver::PROT_NUMBER = 0x0806;

NS_OBJECT_ENSURE_REGISTERED (MacResolver);

TypeId 
MacResolver::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MacResolver")
    .SetParent<Object> ()
    .AddConstructor<MacResolver> ()
    .AddAttribute ("CacheList",
                   "The list of ARP caches",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&MacResolver::m_cacheList),
                   MakeObjectVectorChecker<ArpCache> ())
    .AddTraceSource ("Drop",
                     "Packet dropped because not enough room in pending queue for a specific cache entry.",
                     MakeTraceSourceAccessor (&MacResolver::m_dropTrace))
    ;
  return tid;
}

MacResolver::MacResolver ()
{
  NS_LOG_FUNCTION (this);
}

MacResolver::~MacResolver ()
{
  NS_LOG_FUNCTION (this);
}

void 
MacResolver::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
}

void
MacResolver::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node> ();
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}

void 
MacResolver::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (CacheList::iterator i = m_cacheList.begin (); i != m_cacheList.end (); ++i)
    {
      Ptr<ArpCache> cache = *i;
      cache->Dispose ();
    }
  m_cacheList.clear ();
  m_node = 0;
  Object::DoDispose ();
}

void
MacResolver::AddCache (const Ptr<ArpCache> &cache)
{
  NS_LOG_FUNCTION (this << cache);
  m_cacheList.push_back (cache);
}

Ptr<ArpCache>
MacResolver::FindCache (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  for (CacheList::const_iterator i = m_cacheList.begin (); i != m_cacheList.end (); i++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }

  NS_FATAL_ERROR ("Unreachable code -- "
                  "please file a bug report if this is ever hit");
  return 0;
}

void 
MacResolver::Receive(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                       const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << device << p << protocol << from << to << packetType);

  ArpHeader arp;
  uint32_t size = p->PeekHeader (arp);

  if (size == 0)
    {
      NS_LOG_WARN ("MacResolver: Cannot remove ARP header");
      return;
    }
    
  if (!arp.IsReply () || arp.GetDestinationHardwareAddress () != device->GetAddress ())
    {
      // we listen only ARP-reply for this device
      return;
    }

  Ptr<ArpCache> cache = FindCache (device);
  Ptr<Ipv4Interface> ipv4if = cache->GetInterface ();

  Ipv4Address destination = arp.GetDestinationIpv4Address ();
  
  for (uint32_t i = 0, count = ipv4if->GetNAddresses (); i < count; i++)
    {
      if (destination.IsEqual (ipv4if->GetAddress (i).GetLocal ())) 
        {
          Ipv4Address source = arp.GetSourceIpv4Address ();
          ArpCache::Entry *entry = cache->Lookup (source);
          if (entry != 0)
            {
              if (entry->IsAlive ())
                {
                  Ptr<Interface> mplsIf = ipv4if->GetObject<Interface> ();
                  NS_ASSERT (mplsIf != 0);
                  
                  mplsIf->Flush (source);
                }
            }
          return;
        }
    }
    
              if (entry->IsWaitReply ()) 
                {
                  Ptr<Interface> interface = m_mpls->GetInterfaceForDevice (device);
                  
                  NS_LOG_LOGIC ("MacResolver: node " << m_node->GetId () << 
                                " ifIndex " << interface->GetIfIndex () <<
                                ", address for " << source << " is resolved -- flush");

                  Address from_mac = arp.GetSourceHardwareAddress ();
                  interface->SendArp (source);
                  
                  Ptr<Packet> pending = entry->DequeuePending();
                  while (pending != 0)
                    {
                      cache->GetInterface ()->Send (pending,
                                                arp.GetSourceIpv4Address ());
                      pending = entry->DequeuePending();
                    }
                } 
              else 
                {
                  NS_LOG_LOGIC("MacResolver: node " << m_node->GetId () << 
                               ", got reply from " << source <<
                               " for non-waiting entry -- drop");
                  m_dropTrace (packet);
                }
            }
          return;
        }
    }
}

bool 
MacResolver::Lookup (Ptr<Packet> &packet, const Ipv4Address &destination, 
    const Ptr<ArpCache> &cache, Address *hardwareDestination)
{
  NS_LOG_FUNCTION (this << packet << destination << cache);
  
  ArpCache::Entry *entry = cache->Lookup (destination);
  
  if (entry != 0)
    {
      if (entry->IsExpired ())
        {
          if (entry->IsDead ()) 
            {
              NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                            ", dead entry for " << destination << 
                            " expired -- send arp request");
              entry->MarkWaitReply (packet);
              SendArpRequest (cache, destination);
            }
          else if (entry->IsAlive ()) 
            {
              NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                            ", alive entry for " << destination << 
                            " expired -- send arp request");
              entry->MarkWaitReply (packet);
              SendArpRequest (cache, destination);
            } 
          else if (entry->IsWaitReply ()) 
            {
              NS_FATAL_ERROR ("Test for possibly unreachable code -- "
                  "please file a bug report, with a test case, if this is ever hit");
            }
        }
      else 
        {
          if (entry->IsDead ()) 
            {
              NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                            ", dead entry for " << destination << 
                            " valid -- drop");
              
            } 
          else if (entry->IsAlive ()) 
            {
              NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                            ", alive entry for " << destination << 
                            " valid -- send");
              *hardwareDestination = entry->GetMacAddress ();
              return true;
            }
          else if (entry->IsWaitReply ()) 
            {
              NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                            ", wait reply for " << destination << 
                            " valid -- drop previous");
              if (!entry->UpdateWaitReply (packet))
                {
                  
                }
            }
        }
    }
  else
    {
      NS_LOG_DEBUG ("MacResolver: node " << m_node->GetId () <<
                    ", no entry for " << destination << 
                    " -- send arp request");
      entry = cache->Add (destination);
      entry->MarkWaitReply (packet);
      SendArpRequest (cache, destination);
    }
  return false;
}

void
MacResolver::SendArpRequest (Ptr<const ArpCache> cache, Ipv4Address destination)
{
  NS_LOG_FUNCTION (this << cache << destination);
  
  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  Ptr<NetDevice> device = cache->GetDevice ();
  
  Ptr<Packet> packet = Create<Packet> ();
  Ipv4Address source = ipv4->SelectSourceAddress (device, destination, Ipv4InterfaceAddress::GLOBAL);
  
  ArpHeader arp;
  arp.SetRequest (device->GetAddress (), source, device->GetBroadcast (), destination);
  
  NS_LOG_DEBUG ("MacResolver: sending request from node " << m_node->GetId () <<
                " src: " << device->GetAddress () << " / " << source <<
                " dst: " << device->GetBroadcast () << " / " << destination);
                
  packet->AddHeader (arp);
  cache->GetDevice ()->Send (packet, device->GetBroadcast (), PROT_NUMBER);
}

void
MacResolver::SendArpReply (Ptr<const ArpCache> cache, Ipv4Address source, Ipv4Address destination, Address mac)
{
  NS_LOG_FUNCTION (this << cache << source << destination << mac);
  
  Ptr<NetDevice> device = cache->GetDevice ();

  ArpHeader arp;
  arp.SetReply (device->GetAddress (), source, mac, destination);
  
  NS_LOG_DEBUG ("MacResolver: sending reply from node " << m_node->GetId () <<
                " src: " << device->GetAddress () << " / " << source <<
                " dst: " << mac << " / " << destination);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (arp);
  cache->GetDevice ()->Send (packet, mac, PROT_NUMBER);
}

}//namespace ns3
