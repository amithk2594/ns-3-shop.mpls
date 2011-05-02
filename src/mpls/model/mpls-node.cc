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
  std::clog << Simulator::Now ().GetSeconds () << " [node " << GetId () << "] ";

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/callback.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "mpls-protocol.h"

NS_LOG_COMPONENT_DEFINE ("MplsNode");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MplsNode);

using namespace mpls;
using namespace traces;

TypeId
MplsNode::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MplsNode")
    .SetParent<Node> ()
    .AddConstructor<MplsNode> ()
    .AddAttribute ("LabelSpaceType",
                   "Label space type to use.",
                   EnumValue (PLATFORM),
                   MakeEnumAccessor (&MplsNode::SetLabelSpaceType),
                   MakeEnumChecker (PLATFORM, "Platform",
                                    INTERFACE, "Interface"))
    .AddAttribute ("MinLabelValue", 
                   "The minimum label value.",
                   UintegerValue (0x1000),
                   MakeUintegerAccessor (&MplsNode::SetMinLabelValue),
                   MakeUintegerChecker<uint32_t> (0x10, 0xfffff))
    .AddAttribute ("MaxLabelValue", 
                   "The maximum label value.",
                   UintegerValue (0xfffff),
                   MakeUintegerAccessor (&MplsNode::SetMaxLabelValue),
                   MakeUintegerChecker<uint32_t> (0x10, 0xfffff))
    .AddTraceSource ("Tx", "Send packet to outgoing interface.",
                        MakeTraceSourceAccessor (&MplsNode::m_txTrace))
    .AddTraceSource ("Rx", "Receive packet from incoming interface.",
                        MakeTraceSourceAccessor (&MplsNode::m_rxTrace))
    .AddTraceSource ("Drop", "Drop packet",
                        MakeTraceSourceAccessor (&MplsNode::m_dropTrace))
  ;
  return tid;
}

MplsNode::MplsNode ()
  : m_labelSpaceType (PLATFORM)
{
  NS_LOG_FUNCTION (this);
}

MplsNode::~MplsNode ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MplsNode::NotifyNewAggregate ()
{
  // if (m_ipv4 == 0)
    // {
      // Ptr<Ipv4> ipv4 = this->GetObject<Ipv4> ();
      // if (ipv4 != 0)
        // {
          // m_ipv4 = DynamicCast<mpls::Ipv4Protocol> (ipv4);
          // NS_ASSERT_MSG (m_ipv4 != 0, "Use mpls::Ipv4Protocol instead of default Ipv4");
        // }
    // }

  Object::NotifyNewAggregate ();
}

void
MplsNode::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Object::DoDispose ();
}

LabelSpace*
MplsNode::GetLabelSpace (uint32_t ifIndex)
{
  if (m_labelSpaceType == PLATFORM) {
    return &m_labelSpace;
  }
  return GetInterface (ifIndex)->GetLabelSpace ();
}

void
MplsNode::SetLabelSpaceType (LabelSpaceType type)
{
  if (type == PLATFORM)
    {
      for (InterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
        {
          NS_ASSERT_MSG (!i->GetLabelSpace ()->IsUsed (), "Clear interface label space before set type");
        }
    }
  else
    {
      NS_ASSERT_MSG (!i->GetLabelSpace ()->IsUsed (), "Clear platform label space before set type");
    }
  m_labelSpaceType = type;
}

void
MplsNode::SetMinLabelValue (uint32_t value)
{
  NS_ASSERT_MSG (!m_labelSpace.IsUsed (), "Clear platform label space before set new range");
  m_labelSpace.SetMinValue (value);
}

void
MplsNode::SetMaxLabelValue (uint32_t value)
{
  NS_ASSERT_MSG (!m_labelSpace.IsUsed (), "Clear platform label space before set new range");
  m_labelSpace.SetMaxValue (value);
}

MplsNode::IlmTable*
MplsNode::GetIlmTable (void) const
{
  return &m_ilmTable;
}

MplsNode::FtnTable*
MplsNode::GetFtnTable (void) const
{
  return &m_ftnTable;
}

Ptr<IncomingLabelMap>
MplsNode::LookupIlm (Label label, int32_t interface)
{
  NS_LOG_FUNCTION (this << label << interface);

  for (IlmTable::const_iterator i = m_ilmTable.begin (); i != m_ilmTable.end (); ++i)
    {
      if ((*i)->GetLabel () == label && (*i)->GetInterface () == interface)
        {
          return *i;
        }
    }

  for (IlmTable::const_iterator i = m_ilmTable.begin (); i != m_ilmTable.end (); ++i)
    {
      if ((*i)->GetLabel () == label && (*i)->GetInterface () < 0)
        {
          return *i;
        }
    }

  return 0;
}

Ptr<FecToNhlfe>
MplsNode::LookupFtn (PacketDemux &demux)
{
  NS_LOG_FUNCTION (this);

  for (FtnTable::const_iterator i = m_ftnTable.begin (); i != m_ftnTable.end (); ++i)
    {
      if (((*i)->GetFec ()) (demux))
        {
          return *i;
        }
    }

  return 0;
}

} // namespace mpls
} // namespace ns3
