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
#include "ns3/object.h"
#include "mpls-ilm-helper.h"

NS_LOG_COMPONENT_DEFINE ("MplsIlmHelper");

namespace ns3 {

MplsIlmHelper::MplsIlmHelper ()
{
}

MplsIlmHelper::MplsIlmHelper (const Ptr<Node> &node)
  : _MplsNodeHelper (node)
{
}

MplsIlmHelper::MplsIlmHelper (const std::string &node)
  : _MplsNodeHelper (node)
{
}

MplsIlmHelper::~MplsIlmHelper()
{
}

Ptr<mpls::IlmTable>
MplsIlmHelper::GetIlmTable (void) const
{
  return GetMpls ()->GetIlmTable ();
}

void 
MplsIlmHelper::SetIlmTable (const Ptr<mpls::IlmTable> &table) 
{
  NS_ASSERT (table != 0);
  GetMpls ()->SetIlmTable (table);  
}

Ptr<mpls::IncomingLabelMap> 
MplsIlmHelper::GetIlm (const uint32_t index) const
{
  return GetIlmTable ()->GetIlm (index);
}

void
MplsIlmHelper::RemoveIlm (const uint32_t index)
{
  GetIlmTable ()->RemoveIlm (index);
}

void 
MplsIlmHelper::ClearIlmTable ()
{
  GetIlmTable ()->Clear ();
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe, policy.CreatePolicy ());
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
  const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.CreatePolicy ());
  ilm->AddNhlfe (nhlfe2);
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.CreatePolicy ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.CreatePolicy ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);  
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.CreatePolicy ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.CreatePolicy ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  return GetIlmTable ()->AddIlm (ilm);
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe)
{
  return AddIlm (interface, label, nhlfe, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe)
{
  return AddIlm (label, nhlfe, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2)
{
  return AddIlm (label, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe, policy);
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, policy);
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3,
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, policy);
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, policy);
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, policy);
}

uint32_t
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, const mpls::Nhlfe &nhlfe3, 
    const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, policy);
}

} // namespace ns3
