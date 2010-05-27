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

#include "mpls-forwarding-information.h"

namespace ns3 {
namespace mpls {

MplsForwardingInformation::MplsForwardingInformation ()
{
}

MplsForwardingInformation::~MplsForwardingInformation ()
{
  for (NhlfeList::iterator i = m_nhlfeList.begin (); i != m_nhlfeList.end (); ++i)
    {
      *i = 0;
    }
  m_nhlfeList.clear ();
}

MplsForwardingInformation::AddNhlfe (const Ptr<MplsNhlfe> &nhlfe)
{
  m_nhlfeList.push_back (nhlfe);
}

void
MplsForwardingInformation::RemoveNhlfe (const Ptr<MplsNhlfe> &nhlfe)
{
  m_nhlfeList.remove (nhlfe);
}

void
MplsForwardingInformation::RemoveNhlfe (uint16_t lspid)
{
  for (NhlfeList::iterator i = m_nhlfeList.begin (); i != m_nhlfeList.end (); ++i)
    {
      if ((*i)->GetLspId() == lspid)
        {
          m_nhlfeList.erase (i);
          break;
        }
    }
}

const Ptr<MplsNhlfe>&
MplsForwardingInformation::GetNhlfe (uint16_t lspid)
{
  for (NhlfeList::const_iterator i = m_nhlfeList.begin (); i != m_nhlfeList.end (); ++i)
    {
      if ((*i)->GetLspId() == lspid)
        {
          return &(*i);
        }
    }

  return 0;
}

std::ostream& operator<< (std::ostream& os, const MplsForwardingInformation &info)
{
  info.Print (os);
  return os;
}

} // namespace mpls
} // namespace ns3
