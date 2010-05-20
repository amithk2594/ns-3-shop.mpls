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

#include <iomanip>

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/address.h"

#include "protocol-data-unit.h"
#include "ldp-status-codes.h"

NS_LOG_COMPONENT_DEFINE ("LdpProtocolDataUnit");

namespace ns3 {
namespace ldp {

void
TypeLengthValue::Register (const uint16_t &type, Callback <Ptr<TypeLengthValue> > cb)
{
  GetRegistredTypesMap ().insert(std::make_pair(type, cb));
  //std::make_pair (priority, routingProtocol
}

TypeLengthValue::RegistredTypesMap&
TypeLengthValue::GetRegistredTypesMap (void)
{
  static RegistredTypesMap registred;
  return registred;
}

Ptr<TypeLengthValue>
TypeLengthValue::CreateTLV (uint16_t type)
{
  RegistredTypesMap v = GetRegistredTypesMap ();
  RegistredTypesMap::const_iterator i = v.find (type);
  if (i != v.end ())
    {
      return (*i).second ();
    }

  return 0;
}

TypeLengthValue::TypeLengthValue ()
  : m_unknown (false),
    m_forward (false)
{
}

TypeLengthValue::~TypeLengthValue ()
{
}

bool
TypeLengthValue::IsUnknown (void) const
{
  return m_unknown;
}

void
TypeLengthValue::SetUnknown (bool unknown)
{
  m_unknown = unknown;
}

bool
TypeLengthValue::IsForward (void) const
{
  return m_forward;
}

void
TypeLengthValue::SetForward (bool forward)
{
  m_forward = forward;
}

void
TypeLengthValue::PrintTlv (std::ostream &os, const char* name) const
{
  os << "[" << name << "] - ";

  if (m_unknown)
    {
      os << "U ";
    }

  if (m_unknown)
    {
      os << "U ";

      if (m_forward)
        {
          os << "F ";
        }
    }

  os << "type=0x" << std::hex << GetType () << std::dec << " length=" << GetLength () << " ";
}

UnknownTLV::UnknownTLV ()
  : m_type (0),
    m_buffer ()
{
}

UnknownTLV::UnknownTLV (uint32_t type)
  : m_type (type),
    m_buffer ()
{
}

UnknownTLV::~UnknownTLV ()
{
}

uint32_t
UnknownTLV::GetLength (void) const
{
  return m_buffer.GetSize ();
}

void
UnknownTLV::SetType (uint16_t type)
{
  m_type = type;
}

uint16_t
UnknownTLV::GetType (void) const
{
  return m_type;
}

void
UnknownTLV::SerializeValue (Buffer::Iterator &start) const
{
  start.Write (m_buffer.Begin (), m_buffer.End ());
}

bool
UnknownTLV::DeserializeValue (Buffer::Iterator start, uint32_t size)
{
  m_buffer.AddAtStart (size);
  Buffer::Iterator end = start;
  end.Next (size);
  m_buffer.Begin ().Write (start, end);
  return true;
}

void
UnknownTLV::Print (std::ostream &os) const
{
  PrintTlv (os, "Unknown TLV");
  os << std::endl;
}

/**
 * Message
 */

const uint16_t Message::UNKNOWN_MESSAGE        = 0x0000;
const uint16_t Message::NOTIFICATION_MESSAGE   = 0x0001;
const uint16_t Message::HELLO_MESSAGE          = 0x0100;
const uint16_t Message::INITIALIZATION_MESSAGE = 0x0200;
const uint16_t Message::KEEPALIVE_MESSAGE      = 0x0201;

Message::Message ()
  : m_unknown (false),
    m_type (0),
    m_messageId (0)
{
}

Message::Message (uint16_t type)
  : m_unknown (false),
    m_type (type),
    m_messageId (0),
    m_ldpId (0)
{
}

Message::~Message ()
{
  for (ValueVector::iterator i = m_values.begin (); i != m_values.end (); ++i)
    {
      *i = 0;
    }

  m_values.clear ();
}

void
Message::SetUnknown (bool unknown)
{
  m_unknown = unknown;
}

bool
Message::IsUnknown (void) const
{
  return m_unknown;
}

uint16_t
Message::GetMessageType (void) const
{
  return m_type;
}

void
Message::SetMessageType (uint16_t type)
{
  m_type = type;
}

uint32_t
Message::GetMessageId (void) const
{
  return m_messageId;
}

void
Message::SetMessageId (uint32_t messageId)
{
  m_messageId = messageId;
}

void
Message::AddValue (Ptr<const TypeLengthValue> tlv)
{
  m_values.push_back (tlv);
}

Ptr<const TypeLengthValue>
Message::GetValue (uint32_t i) const
{
  NS_ASSERT_MSG (i < m_values.size (), "Message::GetValue (): value index out of range");
  return m_values[i];
}

uint32_t
Message::GetNValues (void) const
{
  return m_values.size ();
}

Message::Iterator
Message::Begin (void) const
{
  return m_values.begin ();
}

Message::Iterator
Message::End (void) const
{
  return m_values.end ();
}

uint32_t
Message::GetLength (void) const
{
  uint32_t length = 4;
  for (ValueVector::const_iterator i = m_values.begin (); i != m_values.end (); ++i)
    {
      length += 4 + (*i)->GetLength ();
    }

  return length;
}

void
Message::Serialize (Buffer::Iterator &start) const
{
  uint16_t type = m_type;

  if (m_unknown)
    {
      type |= 0x8000;
    }

  start.WriteHtonU16 (type);
  start.WriteHtonU16 (GetLength ());
  start.WriteHtonU32 (m_messageId);

  for (ValueVector::const_iterator i = m_values.begin (); i != m_values.end (); ++i)
    {
      uint16_t type = (*i)->GetType ();
      if ((*i)->IsUnknown ())
        {
          type |= 0x8000;
          if ((*i)->IsForward ())
            {
              type |= 0x4000;
            }
        }

      start.WriteHtonU16 (type);
      start.WriteHtonU16 ((*i)->GetLength ());

      (*i)->SerializeValue (start);
    }
}

void
Message::Print (std::ostream &os) const
{
  os << "Message type=" << std::hex << m_type << std::dec << " id=" << m_messageId << std::endl;
  for (ValueVector::const_iterator i = m_values.begin (); i != m_values.end (); ++i)
    {
      os << "   ";
      (*i)->Print (os);
    }
}

std::ostream&
operator<< (std::ostream& os, const Message &message)
{
  message.Print (os);
  return os;
}

/**
 * PDU Writer
 */

PduWriter::PduWriter ()
  : m_ldpid(0)
{}

PduWriter::~PduWriter ()
{
  Clear ();
}

void
PduWriter::Clear (void)
{
  for (MessageVector::iterator i = m_messages.begin (); i != m_messages.end (); ++i)
    {
      *i = 0;
    }

  m_messages.clear ();
}

void
PduWriter::SetLdpId (uint32_t id)
{
  m_ldpid = id;
}

void
PduWriter::AddMessage (Ptr<const Message> message)
{
  m_messages.push_back (message);
}

uint32_t
PduWriter::GetLength (void) const
{
  uint32_t length = 10;
  for (MessageVector::const_iterator i = m_messages.begin (); i != m_messages.end (); ++i)
    {
      length += 4 + (*i)->GetLength ();
    }

  return length;
}

void
PduWriter::WriteTo (Buffer::Iterator start)
{
  NS_ASSERT_MSG (m_messages.size (), "PduWriter::WriteTo (): no messages to serialize");

  uint32_t length = GetLength ();

  NS_ASSERT_MSG (length <= 4096, "PduWriter::WriteTo (): exceeded PDU size limit");

  Serialize (start, length);
  Clear ();
}

Ptr<Packet>
PduWriter::WriteTo (Ptr<Packet> packet)
{
  NS_ASSERT_MSG (m_messages.size (), "PduWriter::WriteTo (): no messages to serialize");

  uint32_t length = GetLength ();

  NS_ASSERT_MSG (length <= 4096, "PduWriter::WriteTo (): exceeded PDU size limit");

  PduWriter::PacketHeader header (MakeCallback (&PduWriter::Serialize, this), length);
  packet->AddHeader (header);
  Clear ();
  return packet;
}

Ptr<Packet>
PduWriter::Write (void)
{
  return WriteTo (Create<Packet> ());
}

void
PduWriter::Serialize (Buffer::Iterator start, uint32_t length) const
{
  start.WriteHtonU16 (1); // version
  start.WriteHtonU16 (length); // PDU length
  start.WriteHtonU32 (m_ldpid); // ldp identifier (first 4 octets)
  start.WriteU16 (0); // label space, zero due to platform-wide label space

  for (MessageVector::const_iterator i = m_messages.begin (); i != m_messages.end (); ++i)
    {
      (*i)->Serialize (start);
    }
}

TypeId
PduWriter::PacketHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ldp::PduWriter::PacketHeader")
    .AddConstructor<PduWriter::PacketHeader> ()
    .SetParent<Header> ()
    ;
  return tid;
}

TypeId
PduWriter::PacketHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

PduWriter::PacketHeader::PacketHeader ()
  : m_size (0)
{
}

PduWriter::PacketHeader::PacketHeader (DeserializeCallback cb, uint32_t size)
  : m_deserializeCallback (cb),
    m_size (size)
{
}

PduWriter::PacketHeader::~PacketHeader ()
{
}

void
PduWriter::PacketHeader::SetSerializedSize (uint32_t size)
{
  m_size = size;
}

uint32_t
PduWriter::PacketHeader::GetSerializedSize (void) const
{
  return m_size;
}

void
PduWriter::PacketHeader::SetDeserializeCallback (DeserializeCallback cb)
{
  m_deserializeCallback = cb;
}

void
PduWriter::PacketHeader::Serialize (Buffer::Iterator start) const
{
  m_deserializeCallback (start, m_size - 4);
}

uint32_t
PduWriter::PacketHeader::Deserialize (Buffer::Iterator start)
{
  return 0;
}

void
PduWriter::PacketHeader::Print (std::ostream &os) const
{
}

/**
 * PDU Reader
 */

PduReader::PduReader ()
  : m_buffer (),
    m_size (0),
    m_errno (0)
{
}

PduReader::~PduReader ()
{
}

void PduReader::Reset (void)
{
  m_size = 0;
  m_buffer.RemoveAtEnd (m_buffer.GetSize ());
  m_errno = 0 ;
  m_messages.clear ();
}

void
PduReader::Feed (Buffer::Iterator start, Buffer::Iterator end)
{
  uint32_t feedSize = end.GetDistanceFrom (start);
  m_buffer.AddAtEnd (feedSize);

  Buffer::Iterator i = m_buffer.End ();
  i.Prev (feedSize);
  i.Write (start, end);

  ReadPdu ();
}

void
PduReader::Feed (uint8_t *buffer, uint32_t size)
{
  m_buffer.AddAtEnd (size);
  Buffer::Iterator i = m_buffer.End ();
  i.Prev (size);
  i.Write (buffer, size);

  ReadPdu ();
}

Ptr<const Message>
PduReader::GetNextMessage (void)
{
  if (m_messages.size () == 0)
    {
      return 0;
    }

  MessageInfo msg = m_messages.front ();
  m_messages.pop_front ();
  m_errno = msg.errno;
  m_ldpid = msg.ldpid;
  return msg.message;
}

uint32_t
PduReader::GetLastError (void) const
{
  return m_errno;
}

uint32_t
PduReader::GetLdpId (void) const
{
  return m_ldpid;
}

void
PduReader::NotifyMessage (uint32_t ldpid, Ptr<const Message> message)
{
  MessageInfo msg;
  msg.message = message;
  msg.errno = 0;
  msg.ldpid = ldpid;
  m_messages.push_back (msg);
}

void
PduReader::NotifyError (uint32_t ldpid, uint32_t errno)
{
  m_size = 0;
  m_buffer.RemoveAtEnd (m_buffer.GetSize ());
  MessageInfo msg;
  msg.message = 0;
  msg.errno = errno;
  msg.ldpid = ldpid;
  m_messages.push_back (msg);
}

void
PduReader::ReadPdu (void)
{
  uint32_t bufferSize = m_buffer.GetSize ();

  while (bufferSize >= 4)
    {
      if (m_size == 0)
      {
        Buffer::Iterator i = m_buffer.Begin ();

        if (i.ReadNtohU16 () != 1)
          {
            NotifyError (-1, LdpStatusCodes::BAD_PROTOCOL_VERSION);
            return;
          }

        m_size = i.ReadNtohU16 ();

        if (m_size < 14 || m_size > 4096)
          {
            NotifyError (-1, LdpStatusCodes::BAD_PDU_LENGTH);
            return;
          }
      }

      if (bufferSize < m_size + 4)
        {
          break;
        }

      Buffer::Iterator i = m_buffer.Begin ();
      i.Next (4);

      if (!HandlePdu (i, m_size))
        {
          return;
        }

      m_buffer.RemoveAtStart (m_size + 4);
      m_size = 0;
      bufferSize = m_buffer.GetSize ();
    }
}

bool
PduReader::HandlePdu (Buffer::Iterator start, uint32_t size)
{
  uint32_t ldpid = start.ReadNtohU32 ();
  start.ReadU16 ();
  size -= 6;

  while (size >= 4) //XXX: ignore this bytes?
    {
      uint16_t type = start.ReadNtohU16 ();
      uint16_t length = start.ReadNtohU16 ();
      size -= 4;

      if (length < 4 || length > size)
        {
          NotifyError (ldpid, LdpStatusCodes::BAD_MESSAGE_LENGTH);
          return false;
        }

      uint32_t messageId = start.ReadNtohU32 ();
      length -= 4;
      size -= 4;

      Ptr<Message> message = Create<Message> ();
      message->SetMessageType (type & 0x7fff);
      message->SetMessageId (messageId);

      if (type & 0x8000)
        {
          message->SetUnknown (true);
        }

      if (!HandleMessage (start, ldpid, message, length))
        {
          return false;
        }

      start.Next (length);
      size -= length;
    }

  return true;
}

bool
PduReader::HandleMessage (Buffer::Iterator start, uint32_t ldpid, Ptr<Message> message, uint32_t size)
{
  while (size >= 4) //XXX: ignore this bytes?
    {
      uint16_t rawtype = start.ReadNtohU16 ();
      uint16_t length = start.ReadNtohU16 ();
      size -= 4;

      if (length > size)
        {
          NotifyError (ldpid, LdpStatusCodes::BAD_TLV_LENGTH);
          return false;
        }

      size -= length;

      uint16_t type = rawtype & 0x3fff;
      bool unknown = rawtype & 0x8000;

      Ptr<TypeLengthValue> tlv = TypeLengthValue::CreateTLV (type);
      if (tlv == 0)
        {
          if (unknown)
            {
              start.Next (length);
              continue;
            }
          tlv = Create<UnknownTLV> (type);
        }

      if (!tlv->DeserializeValue (start, length))
        {
          NotifyError (ldpid, LdpStatusCodes::MALFORMED_TLV_VALUE);
          return false;
        }

      if (unknown)
        {
          tlv->SetUnknown (true);
          if (rawtype & 0x4000)
            {
              tlv->SetForward (true);
            }
        }

      message->AddValue (tlv);

      start.Next (length);
    }

  NotifyMessage (ldpid, message);
  return true;
}

} // namespace ldp
} // namespace ns3
