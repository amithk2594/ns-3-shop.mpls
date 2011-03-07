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

#ifndef LABEL_REQUEST_MESSAGE_H
#define LABEL_REQUEST_MESSAGE_H

#include <ostream>

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

/**
 * \ingroup Lpd
 * The encoding for the Label Request Message is:
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |0|   Label Request (0x0401)    |       Message Length          |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     Message ID                                |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     FEC TLV                                   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                     Optional Parameters                       |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
class LabelRequestMessage : public Message
{
public:
  static const uint16_t MESSAGE_TYPE = 0x0401;

  LabelRequestMessage ();
  virtual ~LabelRequestMessage ();

  void SetFec (Ptr<FecTLV> tlv);
  Ptr<FecTLV> GetFec (void) const;

  void SetHopCount (Ptr<HopCountTLV> hopCount);
  Ptr<HopCountTLV> GetHopCount (void) const;

  void SetPathVector (Ptr<PathVectorTLV> pathVector);
  Ptr<PathVectorTLV> GetPathVector (void) const;

  // Functions defined in base class Message
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeMessage (Buffer::Iterator &start);
  virtual bool DeserializeMessage (Buffer::Iterator &start, uint32_t size, enum PduDecodingErrno &errno);
  virtual void Print (std::ostream &os) const;

private:
  Ptr<FecTLV> m_fec; // mandatory
  Ptr<HopCountTLV> m_hopCount;
  Ptr<PathVectorTLV> m_pathVector;
};

} // namespace ldp
} // namespace ns3

#endif /* REQUEST_LABEL_MESSAGE_H */
