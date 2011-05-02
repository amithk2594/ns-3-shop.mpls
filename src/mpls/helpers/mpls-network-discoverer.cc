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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/channel.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-interface.h"
#include "ns3/mpls-protocol.h"
#include "ns3/loopback-net-device.h"

#include "mpls-network-discoverer.h"

NS_LOG_COMPONENT_DEFINE ("MplsNetworkDiscoverer");

namespace ns3 {

using namespace mpls;

MplsNetworkDiscoverer::MplsNetworkDiscoverer ()
  : m_vertexes (Create<MplsNetworkDiscoverer::Vertexes> ())
{
}

MplsNetworkDiscoverer::~MplsNetworkDiscoverer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

MplsNetworkDiscoverer::MplsNetworkDiscoverer (const MplsNetworkDiscoverer &o)
{
  m_vertexes = o.m_vertexes;
}

MplsNetworkDiscoverer& 
MplsNetworkDiscoverer::operator= (const MplsNetworkDiscoverer &o)
{
  if (this == &o)
    {
      return *this;
    }

  m_vertexes = o.m_vertexes;
  return *this;
}

MplsNetworkDiscoverer::Vertexes::Vertexes ()
  : m_vertexes ()
{
  NS_LOG_FUNCTION (this);
}

MplsNetworkDiscoverer::Vertexes::~Vertexes ()
{
  NS_LOG_FUNCTION (this);
  Clear ();
}

void 
MplsNetworkDiscoverer::Vertexes::Add (const Ipv4Address& addr, const Ptr<MplsNetworkDiscoverer::Vertex> &vertex)
{
  m_vertexes[addr] = vertex;
}

const Ptr<MplsNetworkDiscoverer::Vertex>& 
MplsNetworkDiscoverer::Vertexes::Get (const Ipv4Address &addr)
{
  return m_vertexes[addr];
}

void
MplsNetworkDiscoverer::Vertexes::Clear ()
{
  for (Iterator i = m_vertexes.begin (); i != m_vertexes.end (); ++i)
    {
      if ((*i).second != 0) 
        {
          (*i).second->Clear ();
          (*i).second = 0;
        }
    }
    
  m_vertexes.clear ();
}

MplsNetworkDiscoverer::Vertexes::Iterator
MplsNetworkDiscoverer::Vertexes::Begin (void)
{
  return m_vertexes.begin ();
}

MplsNetworkDiscoverer::Vertexes::Iterator
MplsNetworkDiscoverer::Vertexes::End (void)
{
  return m_vertexes.end ();
}

MplsNetworkDiscoverer::Vertex::Vertex (const Mac48Address& hwaddr, const Ptr<Interface> &interface)
  : m_hwaddr (hwaddr),
    m_interface (interface),
    m_vertexes (Create<MplsNetworkDiscoverer::Vertexes> ())
{
  NS_LOG_FUNCTION (this << hwaddr << interface);
}

MplsNetworkDiscoverer::Vertex::~Vertex ()
{
  NS_LOG_FUNCTION (this);
}

void
MplsNetworkDiscoverer::Vertex::Clear ()
{
  m_interface = 0;
  m_vertexes = 0;
}

const Mac48Address&
MplsNetworkDiscoverer::Vertex::GetHwAddr (void) const
{
  return m_hwaddr;
}

const Ptr<Interface>&
MplsNetworkDiscoverer::Vertex::GetInterface (void) const
{
  return m_interface;
}

const Ptr<MplsNetworkDiscoverer::Vertex>&
MplsNetworkDiscoverer::Vertex::GetVertex (const Ipv4Address &addr)
{
  return m_vertexes->Get (addr);
}

const Ptr<MplsNetworkDiscoverer::Vertexes>&
MplsNetworkDiscoverer::Vertex::GetVertexes (void)
{
  return m_vertexes;
}

void
MplsNetworkDiscoverer::DiscoverNetwork (void)
{
  const NodeContainer& nodes = GetNetworkNodes ();
  
  m_vertexes->Clear ();
  
  std::list<Ptr<Vertex> > cache;
  
  for (NodeContainer::Iterator i = nodes.Begin (), k = nodes.End (); i != k; ++i)
    {
      Ptr<Mpls> mpls = (*i)->GetObject<Mpls> ();
      uint32_t nDevices = (*i)->GetNDevices ();
  
      for (uint32_t j = 0; j < nDevices; ++j)
        {
          Ptr<NetDevice> dev = (*i)->GetDevice (j);
          if (DynamicCast<LoopbackNetDevice> (dev) != 0)
            {
              continue;
            }

          Address addr = dev->GetAddress ();
          if (Mac48Address::IsMatchingType (addr))
            {
              Ptr<Interface> mplsIf = mpls->GetInterfaceForDevice (dev);
              if (mplsIf == 0)
                {
                  mplsIf = mpls->AddInterface (dev);
                }

              Ptr<MplsNetworkDiscoverer::Vertex> vertex = 
                  Create<MplsNetworkDiscoverer::Vertex> (Mac48Address::ConvertFrom (addr), mplsIf);
              
              if (AddVertexes (mplsIf, vertex))
                {
                  cache.push_back (vertex);
                }
            }
        }
    }
    
  for (std::list<Ptr<Vertex> >::iterator i = cache.begin (), k = cache.end (); i != k; ++i)
    {
      Ptr<Interface> mplsIf = (*i)->GetInterface ();
      Ptr<NetDevice> dev1 = mplsIf->GetDevice ();
      Ptr<Channel> channel = dev1->GetChannel ();
      uint32_t nDevices = channel->GetNDevices ();
      
      for (uint32_t j = 0; j < nDevices; ++j)
        {
          Ptr<NetDevice> dev2 = channel->GetDevice (j);

          if (dev1 == dev2) continue;

          if (UpdateVertexes (dev1, dev2, *i)) 
            {
              mplsIf->SetUp ();
            }
          else 
            {
              mplsIf->SetDown ();
            }
        }
    }

  // configure mac resolvers
  for (std::list<Ptr<Vertex> >::iterator i = cache.begin (), k = cache.end (); i != k; ++i)
    {
      Ptr<Interface> mplsIf = (*i)->GetInterface ();
      Ptr<Vertexes> vertexes = (*i)->GetVertexes ();
      mplsIf->RemoveAllAddresses ();
      
      for (Vertexes::Iterator j = vertexes->Begin (), l = vertexes->End (); j != l; ++j)
        {
          mplsIf->AddAddress ((*j).first, (*j).second->GetHwAddr ());
        }
    }  
}

bool
MplsNetworkDiscoverer::AddVertexes (const Ptr<Interface> &mplsIf, 
    const Ptr<MplsNetworkDiscoverer::Vertex>& vertex)
{
  int32_t ipv4if = mplsIf->LookupIpv4Interface ();
  if (ipv4if < 0) 
    {
      return 0;
    }

  Ptr<Ipv4> ipv4 = mplsIf->GetMpls ()->GetIpv4 ();
  
  int32_t nAddresses = ipv4->GetNAddresses (ipv4if);
  for (int32_t i = 0; i < nAddresses; ++i)
    {
      Ipv4Address addr = ipv4->GetAddress (ipv4if, i).GetLocal ();
      if (m_vertexes->Get (addr) != 0)
        {
          NS_FATAL_ERROR ("Network discovery failed -- address " << addr << " already in use");
        }

      m_vertexes->Add (addr, vertex);
    }
    
  return nAddresses > 0;
}

bool
MplsNetworkDiscoverer::UpdateVertexes (const Ptr<NetDevice> &dev1, const Ptr<NetDevice> &dev2, 
  const Ptr<Vertex> &vertex)
{
  Ptr<Node> node = dev2->GetNode ();
  Ptr<Mpls> mpls = node->GetObject<Mpls> ();

  // disable interface, there is no mpls installed 
  if (mpls == 0) return 0;
  
  Ptr<Interface> mplsIf = mpls->GetInterfaceForDevice (dev2);

  // disable interface, looks like another network
  if (mplsIf == 0) return 0;

  int32_t ipv4if = mplsIf->LookupIpv4Interface ();

  // disable interface, there is no ipv4 interface (ldp uses UDP and TCP)
  if (ipv4if < 0) return 0;

  Ptr<Ipv4> ipv4 = mpls->GetIpv4 ();

  int32_t nAddresses = ipv4->GetNAddresses (ipv4if);

  // disable interface, at least one address should be specified
  if (nAddresses <= 0) return 0;
  
  Ptr<Vertexes> vertexes = vertex->GetVertexes ();  
  
  for (int32_t i = 0; i < nAddresses; ++i)
    {
      Ipv4Address addr = ipv4->GetAddress (ipv4if, i).GetLocal ();
      Ptr<Vertex> target = m_vertexes->Get(addr);
      
      // TODO: disable interface, another network
      if (target == 0) 
        {
          return 0;
        }

      NS_ASSERT_MSG (target != vertex, "Should never happen");
      
      vertexes->Add (addr, target);

      NS_LOG_DEBUG ("NetworkDiscoverer: found link " <<
                    "from node" << dev1->GetNode()->GetId() << ":dev" << dev1->GetIfIndex () << " " << vertex->GetHwAddr () << 
                    " to node" << dev2->GetNode()->GetId() << ":dev" << dev2->GetIfIndex () << " " << target->GetHwAddr () << 
                    ", next-hop: " << addr);
    }

  return 1;
}

}// namespace ns3
