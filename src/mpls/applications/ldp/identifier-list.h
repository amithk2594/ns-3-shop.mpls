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

#ifndef IDENTIFIER_LIST_H
#define IDENTIFIER_LIST_H

#include <list>

namespace ns3 {

template <typename I, typename H>
class IdentifierList
{
public:
  inline IdentifierList (I min = 0, I max = 0xffff)
    : m_min (min),
      m_max (max),
      m_first (min),
      m_last (max)
  {
    NS_ASSERT_MSG (min < max, "IdentifierList (): min is more or equal max");
    m_iter = m_list.begin ();
  }

  inline ~IdentifierList () {};

  inline I Allocate (H v = H())
  {
    if (m_first < m_last)
      {
        I i = m_first++;
        m_list.insert (m_iter, std::make_pair (i, v));
        return i;
      }

    NS_ASSERT_MSG (m_list.size () < m_max - m_min, "IdentifierList: limit exceeded");

    m_first = m_min;
    m_iter = m_list.begin ();
    while (m_iter != m_list.end ())
      {
        m_last = (*m_iter).first;
        if (m_first < m_last)
          {
            return Allocate ();
          }
        m_first = m_last + 1;
        m_iter++;
      }

    m_last = m_max;
    return Allocate ();
  }

  inline H Deallocate (I id)
  {
    for (Iterator i = m_list.begin (); i != m_list.end (); ++i)
      {
        if ((*i).first == id)
          {
            H value = (*i).second;
            m_list.erase (i);
            return value;
          }
      }
    return H ();
  }

  inline H Get (I id) const
  {
    for (Iterator i = m_list.begin (); i != m_list.end (); ++i)
      {
        if ((*i).first == id)
          {
            return (*i).second;
          }
      }

    return H();
  }

private:
  typedef typename std::list<std::pair<I, H> >::iterator Iterator;
  typedef std::list<std::pair<I, H> > IdentList;
  I m_min;
  I m_max;
  I m_first;
  I m_last;
  IdentList m_list;
  Iterator m_iter;
};

} // namespace ns3

#endif /* IDENTIFIER_LIST_H */
