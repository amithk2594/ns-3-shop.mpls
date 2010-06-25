/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
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
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "mpls-interface.h"

NS_LOG_COMPONENT_DEFINE ("MplsInterface");

namespace ns3 {
namespace mpls {

TypeId
MplsInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::mpls::MplsInterface")
    .SetParent<Object> ()
    ;
  return tid;
}

MplsInterface::MplsInterface ()
  : m_strict (false),
    m_node (0),
    m_device (0),
{
  NS_LOG_FUNCTION (this);
  m_fib = Create<MplsFib> (this);
}

MplsInterface::~MplsInterface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MplsInterface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  m_fib = 0;
  Object::DoDispose ();
}

void
MplsInterface::SetNode (const Ptr<Node> &node)
{
  m_node = node;
}

void
MplsInterface::SetDevice (const Ptr<NetDevice> &device)
{
  m_device = device;
}

Ptr<NetDevice>
MplsInterface::GetDevice (void) const
{
  return m_device;
}

void
MplsInterface::AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe)
{
  m_fib->AddIlm (label, nhlfe);
}

Ptr<MplsIlm>
MplsInterface::GetIlm (const MplsLabel &label) const
{
  return m_fib->GetIlm (label);
}

void
MplsInterface::RemoveIlm (const MplsLabel &label)
{
  m_fib->RemoveIlm (label);
}

void
MplsInterface::RemoveIlm (const Ptr<MplsIlm> &ilm)
{
  m_fib->RemoveIlm (ilm);
}

void
MplsInterface::AddFtn (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe)
{
  m_fib->AddFtn (fec, nhlfe);
}

Ptr<MplsFtn>
MplsInterface::GetFtn (const Ptr<MplsFec> &fec) const
{
  return m_fib->GetFtn (fec);
}

void
MplsInterface::RemoveFtn (const Ptr<MplsFec> &fec)
{
  m_fib->RemoveFtn (fec);
}

void
MplsInterface::RemoveFtn (const Ptr<MplsFtn> &ftn)
{
  m_fib->RemoveFtn (ftn);
}

void
MplsInterface::RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe)
{
  m_fib->RemoveNhlfe (nhlfe);
}

const Ptr<MplsFib>&
MplsInterface::GetForwardingTable (void) const
{
  return m_fib;
}

void
MplsInterface::Print (std::ostream &os) const
{
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsInterface> &interface)
{
  interface->Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
