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

#include "forwarding-information-base.h"

namespace ns3 {
namespace mpls {

ForwardingInformationBase::ForwardingInformationBase ()
{
}

ForwardingInformationBase::~ForwardingInformationBase ()
{
}

void
ForwardingInformationBase::AddIlm (Label label, const Ptr<NextHopLabelForwardingEntry> &nhlfe)
{
  Ptr<IncomingLabelMap> ilm = GetIlm (label);
  if (ilm == 0)
    {
      ilm = Create<IncomingLabelMap> (label);
      m_ilmTable.push_back (ilm);
    }
  ilm->AddNhlfe (nhlfe);
}

Ptr<IncomingLabelMap>
ForwardingInformationBase::GetIlm (Label label) const
{
  for (IlmTable::const_iterator i = m_ilmTable.begin (); i != m_ilmTable.end ())
    {
      if ((*i)->m_label == label)
        {
          return *i;
        }
    }
  return 0;
}

void
ForwardingInformationBase::RemoveIlm (Label label)
{
  for (IlmTable::iterator i = m_ilmTable.begin (); i != m_ilmTable.end ())
    {
      if ((*i)->m_label == label)
        {
          m_ilmTable.erase (i);
          break;
        }
    }
}

void
ForwardingInformationBase::RemoveIlm (const Ptr<IncomingLabelMap> &ilm)
{
  m_ilmTable.remove (ilm);
}

void
ForwardingInformationBase::AddFtn (const Ptr<MplsFec> &fec, const Ptr<NextHopLabelForwardingEntry> &nhlfe)
{
  Ptr<FecToNhlfe> ftn = GetFtn (fec);
  if (ftn == 0)
    {
      ftn = Create<FecToNhlfe> (fec);
      m_ftnTable.push_back (ftn);
    }

  ftn->AddNhlfe (nhlfe);
}

Ptr<FecToNhlfe>
ForwardingInformationBase::GetFtn (const Ptr<MplsFec> &fec) const
{
  for (FtnTable::const_iterator i = m_ftnTable.begin (); i != m_ftnTable.end ())
    {
      if ((*i)->m_fec == fec)
        {
          return *i;
        }
    }
  return 0;
}

void
ForwardingInformationBase::RemoveFtn (const Ptr<MplsFec> &fec)
{
  for (FtnTable::const_iterator i = m_ftnTable.begin (); i != m_ftnTable.end ())
    {
      if ((*i)->m_fec == fec)
        {
           m_ftnTable.erase (i);
           break;
        }
    }
}

void
ForwardingInformationBase::RemoveFtn (const Ptr<FecToNhlfe> &ftn)
{
  m_ftnTable.remove (ftn);
}

Ptr<NextHopLabelForwardingEntry>
ForwardingInformationBase::GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const
{
  for (FtnTable::const_iterator i = m_ftnTable.begin (); i != m_ftnTable.end ())
    {
      if ((*i)->m_fec->IsMatch (packet, header))
        {
          return (*i)->GetNhlfe (0);
        }
    }

  return 0;
}

Ptr<NextHopLabelForwardingEntry>
ForwardingInformationBase::GetNhfle (const Ptr<const Packet> &packet, const Ipv6Header &header) const
{
  for (FtnTable::const_iterator i = m_ftnTable.begin (); i != m_ftnTable.end ())
    {
      if ((*i)->m_fec->IsMatch (packet, header))
        {
          return (*i)->GetNhlfe (0);
        }
    }

  return 0;
}

void
ForwardingInformationBase::RemoveNhlfe (const Ptr<NextHopLabelForwardingEntry> &nhlfe)
{
  for (IlmTable::iterator i = m_ilmTable.begin (); i != m_ilmTable.end ())
    {
      if ((*i)->RemoveNhlfe (nhlfe))
        {
          if ((*i)->GetNNhlfe () == 0)
            {
              m_ilmTable.erase (i);
            }
          break;
        }
    }
}

void
ForwardingInformationBase::Print (std::ostream &os) const
{
}

std::ostream& operator<< (std::ostream& os, const Ptr<ForwardingInformationBase> &fib)
{
  fib->Print (os);
}

} // namespace mpls
} // namespace ns3
