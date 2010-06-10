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

namespace ns3 {
namespace mpls {

MplsInterfaceForwardingTable::MplsInterfaceForwardingTable (const Ptr<MplsInterface> &interface)
{
  m_interface = interface;
}

MplsInterfaceForwardingTable::~MplsInterfaceForwardingTable ()
{
  m_interface = 0;
}

void
MplsInterfaceForwardingTable::AddIlm (const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe)
{
  Ptr<MplsIlm> ilm = GetIlm (label);
  if (ilm == 0)
    {
      ilm = Create<MplsIlm> (m_interface, label);
      m_ilmTable.push_back (ilm);
    }
  ilm->AddNhlfe (nhlfe);
}

Ptr<MplsIlm>
MplsInterfaceForwardingTable::GetIlm (const MplsLabel &label) const
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
MplsInterfaceForwardingTable::AddFec (const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe)
{
  Ptr<MplsFtn> ftn = GetFec (fec);
  if (ftn == 0)
    {
      ftn = Create<MplsFtn> (fec);
      m_ftnTable.push_back (ftn);
    }
  ftn->AddNhlfe (nhlfe);
}

Ptr<MplsFtn>
MplsInterfaceForwardingTable::GetFec (const Ptr<MplsFec> &fec) const
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

/**
 * \brief Mpls interface
 */
class MplsForwardingTable : public SimpleRefCount<MplsForwardingTable>
{
public:
  /**
   * Create empty FIB
   */
  MplsForwardingTable ();
  virtual ~MplsForwardingTable ();
  /**
   * Add ilm entry
   */
  void AddIlm (const Ptr<MplsInterface> &interface, const MplsLabel &label, const Ptr<MplsNhlfe> &nhlfe);
  /**
   * Add ftn entry
   */
  void AddFec (const Ptr<MplsInterface> &interface, const Ptr<MplsFec> &fec, const Ptr<MplsNhlfe> &nhlfe);

private:
  typedef std::list<Ptr<MplsIlm> > m_ilmTable;
  typedef std::list<Ptr<MplsFtn> > m_ftnTable;
};

} // namespace mpls
} // namespace ns3

#endif /* MPLS_INTERFACE_H */
