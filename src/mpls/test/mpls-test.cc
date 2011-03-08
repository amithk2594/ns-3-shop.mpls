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

#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/address.h"

#include "ns3/mpls-nhlfe.h"

namespace ns3 {
namespace mpls {

class NhlfeTestCase : public TestCase
{
public:
  /**
   * @brief Constructor.
   */
  NhlfeTestCase ();
  /**
   * @brief Destructor.
   */
  virtual ~NhlfeTestCase ();
  /**
   * @brief Run unit tests for this class.
   */
  virtual void DoRun (void);

};

NhlfeTestCase::NhlfeTestCase () :
  TestCase ("Verify the NHLFE")
{
}

NhlfeTestCase::~NhlfeTestCase ()
{
}

void
NhlfeTestCase::DoRun (void)
{
  Nhlfe nhlfe (Swap (18, 20), Ipv4Address ("10.0.0.2"));
  NS_TEST_ASSERT_MSG_EQ (Ipv4Address::IsMatchingType (nhlfe.GetNextHop ()), true, "Invalid ipv4 address??");
  NS_TEST_ASSERT_MSG_EQ (nhlfe.GetInterface (), 0, "Invalid outgoing interface??");
}

static class MplsTestSuite : public TestSuite
{
public:
  MplsTestSuite () :
    TestSuite ("mpls", UNIT)
  {
    AddTestCase (new NhlfeTestCase ());
  }
} g_mplsTestSuite;

} // namespace mpls
} // namespace ns3

