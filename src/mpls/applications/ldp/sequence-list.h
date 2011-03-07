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

#ifndef REQUEST_LIST_H
#define REQUEST_LIST_H

#include <list>

namespace ns3 {
namespace ldp {

template <typename I, typename H>
class SequenceList
{
public:
  inline SequenceList (): m_seqno (0) {};
  ~SequenceList () {};

  inline I Add (H v = H())
  {
    m_list.push_back(std::make_pair(++m_seqno, v));
    return m_seqno;
  }

  inline H Remove (I id)
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
    return H();
  }

  inline H Get (I id) const
  {
    for (ConstIterator i = m_list.begin (); i != m_list.end (); ++i)
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
  typedef typename std::list<std::pair<I, H> >::const_iterator ConstIterator;
  typedef std::list<std::pair<I, H> > List;
  I m_seqno;
  List m_list;
};

} // namespace ldp
} // namespace ns3

#endif /* IDENTIFIER_LIST_H */
