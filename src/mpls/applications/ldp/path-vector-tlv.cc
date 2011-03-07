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

#include "path-vector-tlv.h"

namespace ns3 {
namespace ldp {

PathVectorTLV::PathVectorTLV ()
{
}

PathVectorTLV::~PathVectorTLV ()
{
  m_lsrs.clear ();
}

uint16_t
PathVectorTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
PathVectorTLV::GetLength (void) const
{
  return 4 * m_lsrs.size ();
}

void
PathVectorTLV::AddLsr (uint32_t id)
{
  m_lsrs.push_back (id);
}

uint32_t
PathVectorTLV::GetLsr (uint32_t i) const
{
  NS_ASSERT_MSG (i < m_lsrs.size (), "PathVectorTLV::GetAddress (): lsr index out of range")
  return m_lsrs[i];
}

uint32_t
PathVectorTLV::GetNLsrs (void) const
{
  return m_lsrs.size ();
}

PathVectorTLV::Iterator
PathVectorTLV::Begin (void) const
{
  return m_lsrs.begin ();
}

PathVectorTLV::Iterator
PathVectorTLV::End (void) const
{
  return m_lsrs.end ();
}

void
PathVectorTLV::Serialize (Buffer::Iterator start) const
{
  NS_ASSERT_MSG (m_lsrs.size (), "PathVectorTLV::Serialize (): no lsr to serialize");

  for (LsrsVector::const_iterator i = m_lsrs.begin (); i != m_lsrs.end (); ++i)
    {
      start.WriteHtonU32 (*i);
    }
}

uint32_t
PathVectorTLV::Deserialize (Buffer::Iterator start)
{
}

void
PathVectorTLV::Print (std::ostream &os) const
{
}

}
};  // namespace ns3
