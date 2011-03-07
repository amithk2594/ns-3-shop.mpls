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

#ifndef ADDRESS_LIST_TLV_H
#define ADDRESS_LIST_TLV_H

#include <ostream>
#include <vector>

#include "ns3/address.h"

#include "ldp-header.h"

namespace ns3 {
namespace ldp {

//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |0|0| Address List (0x0101)     |      Length                   |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |     Address Family            |                               |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
//   |                                                               |
//   |                        Addresses                              |
//   ~                                                               ~
//   |                                                               |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class AddressListTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE = 0x0101;

  AddressListTLV ();
  virtual ~AddressListTLV ();

  virtual uint16_t GetType (void) const;  
  virtual uint32_t GetLength (void) const;

  void SetAddressFamily (enum TypeLengthValue::AddressFamily family);
  enum TypeLengthValue::AddressFamily GetAddressFamily (void) const;
    
  void AddAddress (const Address& address);
  Address GetAddress (uint32_t i) const;
  uint32_t GetNAddresses (void) const;
  
  typedef vector<Address>::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;

  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

private:
  typedef vector<Address> AddressVector;
  enum TypeLengthValue::AddressFamily m_addressFamily;
  AddressVector m_addresses;
};

}} // namespace ns3

#endif
