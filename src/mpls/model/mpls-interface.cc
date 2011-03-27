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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

#include "mpls.h"
#include "mpls-interface.h"

NS_LOG_COMPONENT_DEFINE ("mpls::Interface");

namespace ns3 {
namespace mpls {

TypeId
Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::Interface")
    .SetParent<Object> ()
    ;
  return tid;
}

Interface::Interface (int32_t ifIndex)
  : m_node (0),
    m_device (0),
    m_cache (0),
    m_ifup (true),
    m_ifIndex (ifIndex)
{
  NS_LOG_FUNCTION (this);
}

Interface::~Interface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Interface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void
Interface::SetNode (const Ptr<Node> &node)
{
  m_node = node;
  DoSetup ();
}

void
Interface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
  DoSetup ();  
}

void
Interface::SetArpCache (const Ptr<ArpCache> &cache)
{
  m_cache = cache;
}

void
Interface::DoSetup ()
{
  if (m_node == 0 || m_device == 0)
    {
      return;
    }
}

Ptr<NetDevice>
Interface::GetDevice (void)
{
  return m_device;
}

int32_t
Interface::GetIfIndex (void)
{
  return m_ifIndex;
}
  
bool
Interface::IsUp () const
{
  return m_ifup;
}

bool
Interface::IsDown () const
{
  return !m_ifup;
}

void
Interface::SetUp ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = true;
}

void
Interface::SetDown ()
{
  NS_LOG_FUNCTION (this);
  m_ifup = false;
}

void
Interface::Send (const Ptr<Packet>& packet, const Address &dest)
{
  if (Mac48Address::IsMatchingType (dest))
    {
      // I believe you know what are you doing      
      m_device->Send (packet, Mac48Address::ConvertFrom (dest), Mpls::PROT_NUMBER);
    }
  else if (!m_device->NeedsArp()) 
    {
      m_device->Send (packet, m_device->GetBroadcast (), Mpls::PROT_NUMBER);
    }
  else if (Ipv4Address::IsMatchingType (dest))
    {
      Ipv4Address destination = Ipv4Address::ConvertFrom (dest);

      NS_LOG_DEBUG ("MplsInterface: needs ARP for " << destination);

      ipv4if = GetObject<Ipv4Interface> ();
      
      NS_ASSERT_MSG (ipv4if != 0, "MplsInterface: there is no associated ipv4-interface");
      
      if (m_macResolver->Lookup (packet, destination, m_device, ipv4if->GetArpCache (), &hardwareDestination))
        {
          
        }
      
    }
  else 
    {
      NS_ASSERT_MSG (false,
                     "MplsInterface: node " << m_node->GetId() << " ifIndex " << m_ifIndex << 
                     ", needs ARP -- specify next hop address!");    
    }
}

void
Interface::Send (const Ptr<Packet>& packet, const Ipv4Address &destination)
{
  NS_LOG_FUNCTION (this << packet << destination);

  if (m_device->NeedsArp ())
    {
      
     
      Ptr<ArpCache> cache = ipv4if->GetArpCache ();
      ArpCache::Entry *entry = cache->Lookup (destination);

      if (entry == 0)
        {
          NS_LOG_LOGIC ("MplsInterface: node " << m_node->GetId () <<
                        ", no entry for " << destination << " -- send arp request");
          entry = cache->Add (destination);
          entry->MarkWaitReply (packet);
          SendArpRequest (cache, destination);
        }
      else
        {
          if (entry->IsExpired ()) 
            {
              if (entry->IsDead ()) 
                {
                  NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                            ", dead entry for " << destination << " expired -- send arp request");
                  entry->MarkWaitReply (packet);
                  SendArpRequest (cache, destination);
                } 
              else if (entry->IsAlive ()) 
                {
                  NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                            ", alive entry for " << destination << " expired -- send arp request");
                  entry->MarkWaitReply (packet);
                  SendArpRequest (cache, destination);
                } 
              else if (entry->IsWaitReply ()) 
                {
                  NS_FATAL_ERROR ("Test for possibly unreachable code-- please file a bug report, with a test case, if this is ever hit");
                }
            } 
          else 
            {
              if (entry->IsDead ()) 
                {
                  NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                                ", dead entry for " << destination << " valid -- drop");
                  m_dropTrace (packet);
                } 
              else if (entry->IsAlive ()) 
                {
                  NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                                ", alive entry for " << destination << " valid -- send");
	          *hardwareDestination = entry->GetMacAddress ();
                  return true;
                } 
              else if (entry->IsWaitReply ()) 
                {
                  NS_LOG_LOGIC ("node="<<m_node->GetId ()<<
                                ", wait reply for " << destination << " valid -- drop previous");
                  if (!entry->UpdateWaitReply (packet))
                    {
                      m_dropTrace (packet);
                    }
                }
            }
        }

      Address hardwareDestination;
      NS_LOG_LOGIC ("ARP Lookup");
      
      found = arp->Lookup (p, dest, m_device, m_cache, &hardwareDestination);

      if (found)
        {
          NS_LOG_LOGIC ("Address Resolved.  Send.");
          m_device ->Send (p, hardwareDestination, 
                              Ipv4L3Protocol::PROT_NUMBER);
        }
    }
  else
    {
      m_device->Send (packet, m_device->GetBroadcast (), Mpls::PROT_NUMBER);
    }
}

void
Interface::Send (const Ptr<Packet>& packet, const Mac48Address &nextHop)
{
  m_device->Send (packet, nextHop, Mpls::PROT_NUMBER);
}

} // namespace mpls
} // namespace ns3
