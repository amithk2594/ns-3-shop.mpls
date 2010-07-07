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

#include "mpls-fib.h"

namespace ns3 {
namespace mpls {

MplsFib::MplsFib ()
{
}

MplsFib::~MplsFib ()
{
}

void
MplsFib::AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe)
{
  Ptr<MplsIlm> ilm = GetIlm (label);
  if (ilm == 0)
    {
      ilm = Create<MplsIlm> (label);
      m_ilmTable.push_back (ilm);
    }
  ilm->AddNhlfe (nhlfe);
}

Ptr<MplsIlm>
MplsFib::GetIlm (const MplsLabel &label) const
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
MplsFib::RemoveIlm (const MplsLabel &label)
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
MplsFib::RemoveIlm (const Ptr<MplsIlm> &ilm)
{
  m_ilmTable.remove (ilm);
}

void
MplsFib::AddFtn (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe)
{
  Ptr<MplsFtn> ftn = GetFtn (fec);
  if (ftn == 0)
    {
      ftn = Create<MplsFtn> (fec);
      m_ftnTable.push_back (ftn);
    }

  ftn->AddNhlfe (nhlfe);
}

Ptr<MplsFtn>
MplsFib::GetFtn (const Ptr<MplsFec> &fec) const
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
MplsFib::RemoveFtn (const Ptr<MplsFec> &fec)
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
MplsFib::RemoveFtn (const Ptr<MplsFtn> &ftn)
{
  m_ftnTable.remove (ftn);
}

Ptr<MplsNhlfe>
MplsFib::GetNhfle (const Ptr<const Packet> &packet, const Ipv4Header &header) const
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

Ptr<MplsNhlfe>
MplsFib::GetNhfle (const Ptr<const Packet> &packet, const Ipv6Header &header) const
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
MplsFib::RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe)
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
MplsFib::Print (std::ostream &os) const
{
}

std::ostream& operator<< (std::ostream& os, const Ptr<MplsFib> &fib)
{
  fib->Print (os);
}

} // namespace mpls
} // namespace ns3
