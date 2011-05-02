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
#include "mpls-ilm-helper.h"

namespace ns3 {

using namespace mpls;

MplsIlmHelper::~MplsIlmHelper()
{
}

void
MplsIlmHelper::RemoveIlm (const Ptr<IncomingLabelMap> &ilm)
{
  GetNode ()->GetIlmTable ()->remove (ilm);
}

void 
MplsIlmHelper::ClearIlmTable ()
{
  GetNode ()->GetIlmTable ()->clear ();
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe, 
  const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe, policy.Create ());
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2,
  const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);  
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  ilm->AddNhlfe (nhlfe8);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  ilm->AddNhlfe (nhlfe8);
  ilm->AddNhlfe (nhlfe9);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  ilm->AddNhlfe (nhlfe8);
  ilm->AddNhlfe (nhlfe9);
  ilm->AddNhlfe (nhlfe10);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  ilm->AddNhlfe (nhlfe8);
  ilm->AddNhlfe (nhlfe9);
  ilm->AddNhlfe (nhlfe10);
  ilm->AddNhlfe (nhlfe11);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11, const mpls::Nhlfe &nhlfe12,
    const NhlfeSelectionPolicyHelper &policy)
{
  Ptr<mpls::IncomingLabelMap> ilm = Create<mpls::IncomingLabelMap> (interface, label, nhlfe1, policy.Create ());
  ilm->AddNhlfe (nhlfe2);
  ilm->AddNhlfe (nhlfe3);
  ilm->AddNhlfe (nhlfe4);
  ilm->AddNhlfe (nhlfe5);
  ilm->AddNhlfe (nhlfe6);
  ilm->AddNhlfe (nhlfe7);
  ilm->AddNhlfe (nhlfe8);
  ilm->AddNhlfe (nhlfe9);
  ilm->AddNhlfe (nhlfe10);
  ilm->AddNhlfe (nhlfe11);
  ilm->AddNhlfe (nhlfe12);
  GetNode ()->GetIlmTable ()->push_back (ilm);
  return ilm;
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe)
{
  return AddIlm (interface, label, nhlfe, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
  const mpls::Nhlfe &nhlfe3)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, 
                  GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9,
                  GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9,
                  nhlfe10, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9,
                  nhlfe10, nhlfe11, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (uint32_t interface, mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6, 
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11, const mpls::Nhlfe &nhlfe12)
{
  return AddIlm (interface, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9,
                  nhlfe10, nhlfe11, nhlfe12, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe)
{
  return AddIlm (label, nhlfe, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2)
{
  return AddIlm (label, nhlfe1, nhlfe2, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, 
                  GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, 
                  GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, 
                  nhlfe10, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, 
                  nhlfe10, nhlfe11, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, const mpls::Nhlfe &nhlfe6,
    const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const mpls::Nhlfe &nhlfe11, const mpls::Nhlfe &nhlfe12)
{
  return AddIlm (label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, 
                  nhlfe10, nhlfe11, nhlfe12, GetSelectionPolicy ());
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, 
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, 
                  policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, 
                  policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
    const mpls::Nhlfe &nhlfe9,
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9,
                  policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10,
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10, 
                  policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
    const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10, nhlfe11, 
                  policy);
}

Ptr<IncomingLabelMap>
MplsIlmHelper::AddIlm (mpls::Label label, const mpls::Nhlfe &nhlfe1, const mpls::Nhlfe &nhlfe2, 
    const mpls::Nhlfe &nhlfe3, const mpls::Nhlfe &nhlfe4, const mpls::Nhlfe &nhlfe5, 
    const mpls::Nhlfe &nhlfe6, const mpls::Nhlfe &nhlfe7, const mpls::Nhlfe &nhlfe8, 
    const mpls::Nhlfe &nhlfe9, const mpls::Nhlfe &nhlfe10, const mpls::Nhlfe &nhlfe11,
    const mpls::Nhlfe &nhlfe12, const NhlfeSelectionPolicyHelper &policy)
{
  return AddIlm (-1, label, nhlfe1, nhlfe2, nhlfe3, nhlfe4, nhlfe5, nhlfe6, nhlfe7, nhlfe8, nhlfe9, nhlfe10, nhlfe11, 
                  nhlfe12, policy);
}

} // namespace ns3
