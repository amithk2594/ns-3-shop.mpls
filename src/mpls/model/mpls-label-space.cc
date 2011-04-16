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
#include "mpls-label-space.h"

namespace ns3 {
namespace mpls {

LabelSpace::LabelSpace (uint32_t min, uint32_t max)
  : m_min (min),
    m_max (max)
{
  NS_ASSERT (min < max && min >= 0x10 && max < 0x10000);
}

LabelSpace::~LabelSpace ()
{
}

Label 
LabelSpace::Allocate ()
{
  uint32_t value;
  
  if (m_ranges.size ())
    {
      LabelRangeList::iterator i = m_ranges.begin ();
      LabelRange& r = *i;
      if (r.first > m_min)
        {
          value = m_min;        
          if (r.first != value + 1)
            {
              m_ranges.insert (i, std::make_pair (value, value));
            }
          else 
            {
              r.first = value;
            }
        }
      else
        {
          value = r.second + 1;
          ++i;
          if (i == m_ranges.end () || (*i).first != value + 1)
            {
              r.second = value;
            }
          else
            {
              r.second = (*i).second;
              m_ranges.erase (i);
            }
        }
    }
  else
    {
      value = m_min;
      m_ranges.insert (m_ranges.end (), std::make_pair (value, value));
    }

  NS_ASSERT_MSG (value <= m_max, "Cannot allocate label");
  return Label (value);
}

void
LabelSpace::Deallocate (const Label &label)
{
  uint32_t value = label;
  int32_t begin = 0;
  int32_t pos = 0;  
  int32_t end = m_ranges.size () - 1;
  LabelRangeList::iterator i = m_ranges.begin ();
  
  while (begin <= end)
    {
      int32_t newPos = (begin + end) >> 1;
      int32_t delta = newPos - pos;
      pos = newPos;
      
      if (delta > 0)
        {
          while (delta--) ++i;
        }
      else if (delta < 0)
        {
          while (delta++) --i;        
        }
        
      LabelRange& r = *i;
      
      if (r.first > value)
        {
          end = pos - 1;
        }
      else if (r.second < value)
        {
          begin = pos + 1;
        }
      else 
        {
          if (r.first == value || r.second == value)
            {
              if (r.first == value)
                {
                  ++r.first;
                }
              else 
                {
                  --r.second;
                }
                
              if (r.first > r.second)
                {
                  m_ranges.erase (i);
                }
            }
          else
            {
              m_ranges.insert (i, std::make_pair (r.first, value - 1));
              r.first = value + 1;
            }

          return;
        }
    }
}

void
LabelSpace::Clear (void)
{
  m_ranges.clear ();
}

} // namespace mpls
} // namespace ns3
