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

#ifndef STATUS_TLV_H
#define STATUS_TLV_H

#include <ostream>

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

/**
 * \ingroup Ldp
 * Notification messages carry Status TLVs to specify events being signaled.
 *
 * The encoding for the Status TLV is:
 *
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |U|F| Status (0x0300)           |      Length                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     Status Code                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     Message ID                                |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      Message Type             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Read more: http://www.faqs.org/rfcs/rfc3036.html
 *
 */
class StatusTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  StatusTLV ();
  virtual ~StatusTLV ();

  /**
   * \param statusCode value which specifies the status information
   *
   * A Status Code of 0 signals success.
   */
  void SetStatusCode (uint32_t statusCode);
  /**
   * \returns status code value
   */
  uint32_t GetStatusCode (void) const;
  /**
   * \returns true if fatal error bit is set
   *
   * If fatal error bit is set, this is a fatal error notification.
   */
  bool IsFatalError (void) const;
  /**
   * \param forward if true, status forward bit is set
   */
  void SetMessageId (uint32_t messageId);
  /**
   * \returns message id
   */
  uint32_t GetMessageId (void) const;
  /**
   * \returns messageType the type of the peer message to which the Status TLV refers.
   *
   * If non-zero, the type of the peer message to which the Status TLV refers.
   * If zero, the Status TLV does not refer to any specific message type.
   */
  void SetMessageType (uint16_t messageType);
  /**
   * \returns message type
   */
  uint16_t GetMessageType (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint32_t m_statusCode;
  uint32_t m_messageId;
  uint16_t m_messageType;
};

} // namespace ldp
} // namespace ns3

#endif /* STATUS_TLV_H */
