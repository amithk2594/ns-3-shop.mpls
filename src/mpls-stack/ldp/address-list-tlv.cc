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
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"

#include "address-list-tlv.h"

namespace ns3 {
namespace ldp {

AddressListTLV::AddressListTLV ()
  : m_addressFamily (TypeLengthValue::AF_IP)
{
}

AddressListTLV::~AddressListTLV ()
{
  m_addresses.clear ();
}

uint16_t
AddressListTLV::GetType (void) const
{
  return TLV_TYPE;
}

uint32_t
AddressListTLV::GetLength (void) const
{
  switch (m_addressFamily)
    {
      case TypeLengthValue::AF_IP:
        return m_addresses.size () * 4 + 2;
        
      case TypeLengthValue::AF_IP6:
        return m_addresses.size () * 6 + 2;
    }

  return 0;
}

void
AddressListTLV::SetAddressFamily (enum TypeLengthValue::AddressFamily family)
{
  if (family != m_addressFamily)
    {
      m_addresses.clear ();
      m_addressFamily = family;
    }
}

enum TypeLengthValue::AddressFamily
AddressListTLV::GetAddressFamily (void) const
{
  return m_addressFamily;
}

void
AddressListTLV::AddAddress (const Address &address)
{
  switch (m_addressFamily)
    {
      case TypeLengthValue::AF_IP:
        NS_ASSERT_MSG (Ipv4Address::IsMatchingType (address), 
                        "AddressListTLV::AddAddress (): Ipv4 address expected");
        break;
      case TypeLengthValue::AF_IP6:
        NS_ASSERT_MSG (Ipv6Address::IsMatchingType (address), 
                        "AddressListTLV::AddAddress (): Ipv6 address expected");
        break;
    }
  m_addresses.push_back (address);
}

Address
AddressListTLV::GetAddress (uint32_t i) const
{
  NS_ASSERT_MSG (i < m_addresses.size (), "AddressListTLV::GetAddress (): address index out of range")
  return m_addresses[i];
}

uint32_t
AddressListTLV::GetNAddresses (void) const
{
  return m_addresses.size ();
}

AddressListTLV::Iterator 
AddressListTLV::Begin (void) const
{
  return m_addresses.begin ();
}

AddressListTLV::Iterator 
AddressListTLV::End (void) const
{
  return m_addresses.end ();
}

void
AddressListTLV::Serialize (Buffer::Iterator start) const
{
  NS_ASSERT_MSG (m_addresses.size (), "AddressListTLV::Serialize (): no addresses to serialize");
  
  start.WriteHtonU16 (m_addressFamily);
  uint8_t buffer[20];
      
  for (AddressVector::const_iterator i = m_addresses.begin (); i != m_addresses.end (); ++i)
    {
      uint32_t length = (*i).CopyTo (buffer);
      start.Write (buffer, length);
    }
}

uint32_t
AddressListTLV::Deserialize (Buffer::Iterator start)
{
}

void
AddressListTLV::Print (std::ostream &os) const
{
}

}}; // namespace ns3
