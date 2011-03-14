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

#ifndef PROTOCOL_DATA_UNIT_H
#define PROTOCOL_DATA_UNIT_H

#include <vector>
#include <list>
#include <map>
#include <ostream>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/header.h"
#include "ns3/packet.h"
#include "ns3/address.h"

#define ENSURE_REGISTER_TLV(type)                         \
  static struct _TLV##type##_RegistrationClass            \
  {                                                       \
    _TLV##type##_RegistrationClass () {                   \
        TypeLengthValue::Register (                       \
          type::TLV_TYPE,                                 \
          MakeCallback(&type::Factory)                    \
        );                                                \
    }                                                     \
  } _tlv_##type##_RegistrationVariable

namespace ns3 {
namespace ldp {

/**
 * \ingroup Lpd
 * Type-Length-Value Encoding
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |U|F|        Type               |            Length             |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                             Value                             |
 *   ~                                                               ~
 *   |                                                               |
 *   |                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class TypeLengthValue : public SimpleRefCount<TypeLengthValue>
{
public:
  static void Register (const uint16_t &type, Callback <Ptr<TypeLengthValue> > cb);
  static Ptr<TypeLengthValue> CreateTLV (uint16_t type);

  TypeLengthValue ();
  virtual ~TypeLengthValue ();

  /*
   * \param unknown unknown bit value
   */
  void SetUnknown (bool unknown);
  /**
   * \returns true if unknown bit is set
   *
   * if false, a notification must be returned to the message originator and
   * the entire message must be ignored.
   * if true, the unknown TLV is silently ignored and the rest of the message is
   * processed as if the unknown TLV did not exist.
   */
  virtual bool IsUnknown (void) const;
  /*
   * \param forward forward bit value
   */
  void SetForward (bool forward);
  /**
   * \returns true if forward bit is set
   *
   * If is clear, the unknown TLV is not forwarded with the containing message.
   * If F is set, the unknown TLV is forwarded with the containing message.
   */
  virtual bool IsForward (void) const;
  /**
   * \returns message length excluding Message Type and Message Length fields
   */
  virtual uint32_t GetLength (void) const = 0;
  /**
   * \returns TLV type
   */
  virtual uint16_t GetType (void) const = 0;
  /**
   * \param start an iterator which points to where the value should be written.
   * Serialize TLV value
   */
  virtual void SerializeValue (Buffer::Iterator &start) const = 0;
  /**
   * \param start an iterator which points to where the value should written.
   * \param size expected size of the TLV value to be sure that the value is correct.
   * \param errno an pdu decoding error number
   * \returns false if value is malformed
   * Deserialize TLV value
   */
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size) = 0;
  /**
   * \param os output stream
   */
  virtual void Print (std::ostream &os) const = 0;

protected:
  /**
   * \param os output stream
   * \param name type name of TLV
   */
  void PrintTlv (std::ostream &os, const char *name) const;
private:
  typedef std::map<uint16_t, Callback <Ptr<TypeLengthValue> > > RegistredTypesMap;
  static RegistredTypesMap& GetRegistredTypesMap (void);
  bool m_unknown;
  bool m_forward;
};

/**
 * \ingroup Ldp
 * Unknown Tlv holder
 */
class UnknownTLV : public TypeLengthValue
{
public:
  UnknownTLV ();
  UnknownTLV (uint32_t type);
  virtual ~UnknownTLV ();

  void SetType (uint16_t type);

  // Functions defined in base class TypeLengthValue
  virtual uint32_t GetLength (void) const;
  virtual uint16_t GetType (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint16_t m_type;
  Buffer m_buffer;
};

/**
 * \ingroup Ldp
 * LDP Messages
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |U|   Message Type              |      Message Length           |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                     Message ID                                |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   +                                                               +
 *   |                     Mandatory Parameters                      |
 *   +                                                               +
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   +                                                               +
 *   |                     Optional Parameters                       |
 *   +                                                               +
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class Message : public SimpleRefCount<Message>
{
public:
  static const uint16_t UNKNOWN_MESSAGE;
  static const uint16_t NOTIFICATION_MESSAGE;
  static const uint16_t HELLO_MESSAGE;
  static const uint16_t INITIALIZATION_MESSAGE;
  static const uint16_t KEEPALIVE_MESSAGE;

  Message ();
  Message (uint16_t type);

  virtual ~Message ();
  /**
   * \param unknown unknown bit value
   */
  void SetUnknown (bool unknown);
  /**
   * \returns true if unknown bit is set
   * Upon receipt of an unknown message, if bit is clear, a notification is returned to the message originator.
   * if bit is set, the unknown message is silently ignored.
   */
  bool IsUnknown (void) const;
  /**
   * \param type the type of message
   */
  void SetMessageType (uint16_t type);
  /**
   * \returns the type of message
   */
  uint16_t GetMessageType (void) const;
  /**
   * \param messageId message identifier
   */
  void SetMessageId (uint32_t messageId);
  /**
   * \returns messsage identifier
   */
  uint32_t GetMessageId (void) const;
  /**
   * \param tlv add TLV to message
   */
  void AddValue (Ptr<const TypeLengthValue> tlv);
  /**
   * \param i TLV index
   * \returns TLV by index
   */
  Ptr<const TypeLengthValue> GetValue (uint32_t i) const;
  /**
   * \returns number of TLV in message
   */
  uint32_t GetNValues (void) const;

  typedef std::vector<Ptr<const TypeLengthValue> >::const_iterator Iterator;
  Iterator Begin (void) const;
  Iterator End (void) const;

  /**
   * \returns message length
   */
  uint32_t GetLength (void) const;
  /**
   * \param start an iterator which points to where the message body should be written.
   */
  void Serialize (Buffer::Iterator &start) const;
  /**
   * \param os output stream
   */
  void Print (std::ostream &os) const;

private:
  typedef std::vector<Ptr<const TypeLengthValue> > ValueVector;

  bool        m_unknown;    // ignore unknown bit
  uint16_t    m_type;       // message type
  uint32_t    m_messageId;  // message id
  uint32_t    m_ldpId;      // ldp id
  ValueVector m_values; // message parameters
};

std::ostream& operator<< (std::ostream& os, const Message &message);

/**
 * \ingroup Ldp
 * LDP PDUs
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |  Version                      |         PDU Length            |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                         LDP Identifier                        |
 *   +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

class PduWriter : public SimpleRefCount<PduWriter>
{
public:

  PduWriter ();
  virtual ~PduWriter ();

  /**
   * \param id ldp identifier (last 2 octets is zero)
   */
  void SetLdpId (uint32_t id);
  /**
   * \param message add message to Ldp PDU
   */
  void AddMessage (Ptr<const Message> message);
  /**
   * \param packet Packet where PDU should be written
   */
  Ptr<Packet> WriteTo (Ptr<Packet> packet);
  /**
   * \param start an iterator which points to where PDU should be written.
   */
  void WriteTo (Buffer::Iterator start);
  /**
   * \returns Packet
   */
  Ptr<Packet> Write (void);

private:
  class PacketHeader : public Header
  {
  public:
    static TypeId GetTypeId ();
    virtual TypeId GetInstanceTypeId () const;
    typedef Callback<void, Buffer::Iterator, uint32_t> DeserializeCallback;

    PacketHeader ();
    PacketHeader (DeserializeCallback cb, uint32_t size);
    virtual ~PacketHeader ();

    void SetSerializedSize (uint32_t size);
    void SetDeserializeCallback (DeserializeCallback cb);

    uint32_t GetSerializedSize (void) const;
    void Serialize (Buffer::Iterator start) const;
    uint32_t Deserialize (Buffer::Iterator start);
    void Print (std::ostream &os) const;

  private:
    DeserializeCallback m_deserializeCallback;
    uint32_t m_size;
  };

  void Clear (void);
  uint32_t GetLength (void) const;
  void Serialize (Buffer::Iterator start, uint32_t length) const;

  typedef std::vector<Ptr<const Message> > MessageVector;

  MessageVector m_messages;
  uint32_t m_ldpid;
};

class PduReader : public SimpleRefCount<PduReader>
{
public:
  PduReader ();
  virtual ~PduReader ();

  void Reset (void);
  /**
   * \param start the start of the data to feed.
   * \param end the end of the data to feed.
   * \returns false if fatal error.
   */
  void Feed (Buffer::Iterator start, Buffer::Iterator end);
  /**
   * \param buffer a byte buffer to feed.
   * \param size number of bytes to feed.
   * \returns false if fatal error.
   */
  void Feed (uint8_t *buffer, uint32_t size);
  /**
   * \brief Get Next message from the stack
   */
  Ptr<const Message> GetNextMessage (void);
  /**
   * \brief Get last error
   */
  uint32_t GetLastError (void) const;
  /**
   * \brief Get last error
   */
  uint32_t GetLdpId (void) const;

private:
  struct MessageInfo
  {
    uint32_t ldpid;
    Ptr<const Message> message;
    uint32_t errno;
  };
  /**
   * \param error pdu reading (decoding) error
   */
  void NotifyError (uint32_t ldpid, uint32_t errno);
  /**
   * \param error pdu reading (decoding) error
   */
  void NotifyMessage (uint32_t ldpid, Ptr<const Message>);
  /**
   * \brief decode pdu
   */
  void ReadPdu (void);
  /**
   * \param start an iterator which points to where the PDU should written.
   * \param size size of PDU to handle
   * \returns false if fatal error
   */
  bool HandlePdu (Buffer::Iterator start, uint32_t size);
  /**
   * \param start an iterator which points to where the message should written.
   * \param size size of message to handle
   * \returns false if fatal error
   */
  bool HandleMessage (Buffer::Iterator start, uint32_t ldpid, Ptr<Message> message, uint32_t size);

  typedef std::list<MessageInfo> MessageList;

  Buffer       m_buffer; // buffer to hold pdu
  uint32_t     m_size;   // last PDU size
  uint32_t     m_errno;  // last error
  uint32_t     m_ldpid;  // ldp id
  MessageList  m_messages;

};

} // namespace ldp
} // namespace ns3

#endif /* PROTOCOL_DATA_UNIT_H */
