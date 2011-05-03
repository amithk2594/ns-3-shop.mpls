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

#ifndef MPLS_NHLFE_SELECTION_POLICY_H
#define MPLS_NHLFE_SELECTION_POLICY_H

#include <ostream>
#include <vector>
#include "ns3/object.h"
#include "ns3/packet.h"
#include "mpls-nhlfe.h"
#include "mpls-interface.h"

namespace ns3 {
namespace mpls {

class Nhlfe;
class Interface;

/**
 * \ingroup mpls
 * \brief Abstract NHLFE selection policy
 */
class NhlfeSelectionPolicy : public Object
{
public:
  static TypeId GetTypeId (void);
  
  NhlfeSelectionPolicy ();
  virtual ~NhlfeSelectionPolicy ();
  
  /**
   * @brief Returns NHLFE for specified index (called by the Iterator)
   */
  const Nhlfe& Get (const std::vector<Nhlfe> &nhlfe, uint32_t index);
  /**
   * @brief Returns true if nhlfe can be selected
   * @param nhlfe Nhlfe vector
   * @param index Nhlfe index
   * @param iface Outgoing interface
   * @param packet Packet
   */
  bool Select (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
                const Ptr<const Interface> &interface, const Ptr<const Packet> &packet);   
  /**
   * @brief Print policy 
   */
  virtual void Print (std::ostream &os) const;

protected:
  virtual void DoStart (uint32_t size);
  virtual const Nhlfe& DoGet (const std::vector<Nhlfe> &nhlfe, uint32_t index);
  virtual bool DoSelect (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
                          const Ptr<const Interface> &interface, const Ptr<const Packet> &packet);   
  //template <class T> Ptr<Queue> GetQueue (const Ptr<T> &device) { return device->GetQueue (); };
private:
  int32_t m_maxPackets;
  int32_t m_maxBytes;
};

/**
 * \ingroup mpls
 * \brief NHLFE simple Round Robin selection policy
 */
class RoundRobinPolicy : public NhlfeSelectionPolicy
{
public:
  static TypeId GetTypeId (void);
  
  RoundRobinPolicy ();
  virtual ~RoundRobinPolicy ();
  virtual void Print (std::ostream &os) const;

protected:
  virtual const Nhlfe& DoGet (const std::vector<Nhlfe> &nhlfe, uint32_t index);

private:
  uint32_t m_index;
};

/**
 * \ingroup mpls
 * \brief Stefano Avallone NHLFE Round Robin selection policy
 */
class StaRoundRobinPolicy : public NhlfeSelectionPolicy
{
public:
  static TypeId GetTypeId (void);
  
  StaRoundRobinPolicy ();
  virtual ~StaRoundRobinPolicy ();
  virtual void Print (std::ostream &os) const;
  
protected:
  virtual void DoStart (uint32_t size);
  virtual const Nhlfe& DoGet (const std::vector<Nhlfe> &nhlfe, uint32_t index);
  virtual bool DoSelect (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
      const Ptr<const Interface> &interface, const Ptr<const Packet> &packet);   
  
private:
  std::list<uint32_t> m_mapping;
  std::list<uint32_t>::iterator m_iter;
  
};

/**
 * \ingroup mpls
 * \brief NHLFE Weighted selection policy
 */
class WeightedPolicy : public NhlfeSelectionPolicy
{
public:
  static TypeId GetTypeId (void);
  
  WeightedPolicy ();
  virtual ~WeightedPolicy ();
  virtual void Print (std::ostream &os) const;
  
  void SetWeights (const std::vector<double>& weights);
  
  class NhlfeInfo
  {
  public:
    uint32_t m_index;
    double m_currentRatio;
    double m_requiredRatio;
    
    bool DecreasingDiffOrder (const NhlfeInfo& y) const;
  };
  
protected:
  virtual void DoStart (uint32_t size);
  virtual const Nhlfe& DoGet (const std::vector<Nhlfe> &nhlfe, uint32_t index);
  virtual bool DoSelect (const std::vector<Nhlfe> &nhlfe, uint32_t index, 
     const Ptr<const Interface> &interface, const Ptr<const Packet> &packet);   
  
private:
  uint32_t m_Ctot;
  uint32_t m_Cmin;
  uint32_t m_Cmax;
  bool m_byteCounter;
  std::vector<double> m_weights;
    
  std::list<NhlfeInfo> m_mapping;
  std::list<NhlfeInfo>::iterator m_iter;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_NHLFE_SELECTION_POLICY_H */
