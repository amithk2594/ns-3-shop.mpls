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

#ifndef MPLS_NETWORK_DISCOVERER_H
#define MPLS_NETWORK_DISCOVERER_H

#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/net-device.h"
#include "ns3/mpls.h"
#include "ns3/mpls-interface.h"
#include "ns3/sgi-hashmap.h"

#include "mpls-network-helper-base.h"

namespace ns3 {

/**
 * \brief Mpls network configurator
 */
class MplsNetworkDiscoverer : public MplsNetworkHelperBase
{
public:
  /**
   * @brief Create a new discoverer
   */
  MplsNetworkDiscoverer(void);  
  MplsNetworkDiscoverer (const MplsNetworkDiscoverer &o);
  MplsNetworkDiscoverer& operator= (const MplsNetworkDiscoverer &o);

  /**
   * @brief Destroy object
   */
  virtual ~MplsNetworkDiscoverer(void);
  
  void DiscoverNetwork (void);
  
private:
  class Vertex;
  
  class Vertexes : public SimpleRefCount<Vertexes>
  {
  public:
    Vertexes ();
    ~Vertexes ();
    void Add (const Ipv4Address &addr, const Ptr<Vertex> &vertex);
    const Ptr<Vertex>& Get (const Ipv4Address &addr);
    void Clear (void);
  private:
    typedef sgi::hash_map<Ipv4Address, Ptr<Vertex>, Ipv4AddressHash>::iterator Iterator;
    sgi::hash_map<Ipv4Address, Ptr<Vertex>, Ipv4AddressHash> m_vertexes;
  };
  
  class Vertex : public SimpleRefCount<Vertex>
  {
  public:
    Vertex (const Mac48Address& hwaddr, const Ptr<mpls::Interface> &interface);
    ~Vertex ();
    const Mac48Address& GetHwAddr (void) const;
    const Ptr<mpls::Interface>& GetInterface (void) const;
    const Ptr<Vertex>& GetVertex (const Ipv4Address &addr);
    const Ptr<Vertexes>& GetVertexes (void);
    void Clear (void);
  private:
    Mac48Address m_hwaddr;
    Ptr<mpls::Interface> m_interface;
    Ptr<Vertexes> m_vertexes;
  };
  
  bool AddVertexes (const Ptr<mpls::Interface> &mplsIf, const Ptr<MplsNetworkDiscoverer::Vertex>& vertex);
  void UpdateVertexes (const Ptr<mpls::Interface> &iface, const Ptr<NetDevice> &dev1, const Ptr<NetDevice> &dev2,
                         const Ptr<Vertex> &vertex);

  Ptr<Vertexes> m_vertexes;
};

} // namespace ns3

#endif /* MPLS_NETWORK_DISCOVERER_H */

