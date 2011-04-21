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
#include "mpls-nhlfe-selection-policy-helper.h"

namespace ns3 {

NhlfeSelectionPolicyHelper::NhlfeSelectionPolicyHelper ()
{
  m_factory.SetTypeId (GetPolicyTypeId ());
}

NhlfeSelectionPolicyHelper::~NhlfeSelectionPolicyHelper()
{
}

std::string
NhlfeSelectionPolicyHelper::GetPolicyTypeId (void)
{
  return "ns3::mpls::NhlfeSelectionPolicy";
}

void 
NhlfeSelectionPolicyHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

Ptr<mpls::NhlfeSelectionPolicy> 
NhlfeSelectionPolicyHelper::Create (void) const
{
  return m_factory.Create<mpls::NhlfeSelectionPolicy> ();
}

RoundRobinPolicyHelper::RoundRobinPolicyHelper ()
{
}

RoundRobinPolicyHelper::~RoundRobinPolicyHelper ()
{
}

std::string
RoundRobinPolicyHelper::GetPolicyTypeId (void)
{
  return "ns3::mpls::RoundRobinPolicy";
}

StaRoundRobinPolicyHelper::StaRoundRobinPolicyHelper ()
{
}

StaRoundRobinPolicyHelper::~StaRoundRobinPolicyHelper ()
{
}

std::string
StaRoundRobinPolicyHelper::GetPolicyTypeId (void)
{
  return "ns3::mpls::StaRoundRobinPolicy";
}

WeightedSelectionPolicyHelper::WeightedSelectionPolicyHelper ()
{
}

WeightedSelectionPolicyHelper::~WeightedSelectionPolicyHelper ()
{
}

std::string WeightedSelectionPolicyHelper::GetPolicyTypeId (void)
{
  return "ns3::mpls::WeightedSelectionPolicy";
}

Ptr<mpls::NhlfeSelectionPolicy>
WeightedSelectionPolicyHelper::Create (void) const
{
  Ptr<mpls::NhlfeSelectionPolicy> policy = m_factory.Create<mpls::NhlfeSelectionPolicy> ();
  
  policy->GetObject<mpls::WeightedSelectionPolicy> ()->AddWeights (m_weight);
  
  return policy;
}


void WeightedSelectionPolicyHelper::AddWeight(double w)
{
  m_weight.push_back (w);
}

void WeightedSelectionPolicyHelper::AddWeight(double w1, double w2)
{
  m_weight.push_back (w1);
  m_weight.push_back (w2);
}

void WeightedSelectionPolicyHelper::AddWeight(double w1, double w2, double w3)
{
  m_weight.push_back (w1);
  m_weight.push_back (w2);
  m_weight.push_back (w3);
}

void WeightedSelectionPolicyHelper::AddWeight(double w1, double w2, double w3, double w4)
{
  m_weight.push_back (w1);
  m_weight.push_back (w2);
  m_weight.push_back (w3);
  m_weight.push_back (w4);
}

void WeightedSelectionPolicyHelper::AddWeight(double w1, double w2, double w3, double w4, double w5)
{
  m_weight.push_back (w1);
  m_weight.push_back (w2);
  m_weight.push_back (w3);
  m_weight.push_back (w4);
  m_weight.push_back (w5);
}

void WeightedSelectionPolicyHelper::AddWeight(double w1, double w2, double w3, double w4, double w5, double w6)
{
  m_weight.push_back (w1);
  m_weight.push_back (w2);
  m_weight.push_back (w3);
  m_weight.push_back (w4);
  m_weight.push_back (w5);
  m_weight.push_back (w6);
}


} // namespace ns3
