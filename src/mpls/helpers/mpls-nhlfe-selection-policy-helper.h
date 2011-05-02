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

#ifndef MPLS_SELECTION_POLICY_HELPER_H
#define MPLS_SELECTION_POLICY_HELPER_H

#include "ns3/string.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"

#include "ns3/mpls-nhlfe-selection-policy.h"

namespace ns3 {

/**
 * \brief Mpls nhlfe selection policy helper
 */
class NhlfeSelectionPolicyHelper: private ObjectFactory
{
protected:
  using ObjectFactory::Create;
  NhlfeSelectionPolicyHelper (const std::string &id);
public:
  /**
   * @brief Create a new NhlfeSelectionPolicyHelper object
   */
  NhlfeSelectionPolicyHelper ();
  virtual ~NhlfeSelectionPolicyHelper ();
  
  virtual Ptr<mpls::NhlfeSelectionPolicy> Create (void) const;
  void SetAttribute (std::string name, const AttributeValue &value);
};

/**
 * \brief Mpls round robin policy helper
 */
class RoundRobinPolicyHelper : public NhlfeSelectionPolicyHelper
{
public:
  /**
   * @brief Create a new RoundRobinPolicyHelper object
   */
  RoundRobinPolicyHelper ();
  virtual ~RoundRobinPolicyHelper ();
};

/**
 * \brief Mpls Sta round robin policy helper
 */
class StaRoundRobinPolicyHelper : public NhlfeSelectionPolicyHelper
{
public:
  /**
   * @brief Create a new StaRoundRobinPolicyHelper object
   */
  StaRoundRobinPolicyHelper ();
  virtual ~StaRoundRobinPolicyHelper ();
};

/**
 * \brief Mpls weighted selection policy helper
 */
class WeightedPolicyHelper : public NhlfeSelectionPolicyHelper
{
public:
  /**
   * @brief Create a new WeightedSelectionPolicyHelper object
   */
  WeightedPolicyHelper ();
  virtual ~WeightedPolicyHelper ();

  Ptr<mpls::NhlfeSelectionPolicy> Create (void) const;
  
  /**
   * @brief Add weights to be associated with the Nhlfes
   */
  void AddWeight (double w);
  void AddWeight (double w1, double w2);
  void AddWeight (double w1, double w2, double w3);
  void AddWeight (double w1, double w2, double w3, double w4);
  void AddWeight (double w1, double w2, double w3, double w4, double w5);
  void AddWeight (double w1, double w2, double w3, double w4, double w5, double w6);  
  
private:
  std::vector<double> m_weight;
};


} // namespace ns3

#endif /* MPLS_SELECTION_POLICY_HELPER_H */
