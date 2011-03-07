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

#ifndef COMMON_SESSION_PARAMS_TLV_H
#define COMMON_SESSION_PARAMS_TLV_H

#include <ostream>

#include "protocol-data-unit.h"

namespace ns3 {
namespace ldp {

/**
 * \ingroup Ldp
 * Common Session Parameters TLV
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |0|0| Common Sess Parms (0x0500)|      Length                   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   | Protocol Version              |      KeepAlive Time           |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |A|D|  Reserved |     PVLim     |      Max PDU Length           |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                 Receiver LDP Identifier                       |
 *   +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               |
 *   -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++
 */
class CommonSessionParamsTLV : public TypeLengthValue
{
public:
  static const uint16_t TLV_TYPE;
  static Ptr<TypeLengthValue> Factory (void);

  CommonSessionParamsTLV ();
  virtual ~CommonSessionParamsTLV ();

  /**
   * \param time keep-alive time field value
   */
  void SetKeepAliveTime (uint16_t time);
  /**
   * \returns keep-alive time field value
   */
  uint16_t GetKeepAliveTime (void) const;
  /**
   * \param enabled true if Downstream On Demand of Label advertisement is enabled
   */
  void SetDownstreamOnDemand (bool enabled);
  /**
   * \returns true if Downstream On Demand of Label advertisement is enabled
   */
  bool IsDownstreamOnDemand (void) const;
  /**
   * \param enabled if true then loop detection is enabled
   */
  void SetLoopDetection (bool enabled);
  /**
   * \returns true if loop detection enabled
   */
  bool IsLoopDetection (void) const;
  /**
   * \param limit path vector limit
   * The configured maximum path vector length.  Must be 0 if loop detection is disabled.
   */
  void SetPathVectorLimit (uint8_t limit);
  /**
   * \returns path vector limit
   */
  uint8_t GetPathVectorLimit (void) const;
  /**
   * \param length maximum PDU length
   * The maximum allowable length for LDP PDUs for the session. A value of 255 or less
   * specifies the default maximum length of 4096 octets
   */
  void SetMaxPduLength (uint16_t length);
  /**
   * \returns maximum PDU length
   */
  uint16_t GetMaxPduLength (void) const;
  /**
   * \param id identifier of the receiver's label space
   */
  void SetLdpIdentifier (uint32_t id);
  /**
   * \returns identifier of the receiver's label space
   */
  uint32_t GetLdpIdentifier (void) const;

  // Functions defined in base class TypeLengthValue
  virtual uint16_t GetType (void) const;
  virtual uint32_t GetLength (void) const;
  virtual void SerializeValue (Buffer::Iterator &start) const;
  virtual bool DeserializeValue (Buffer::Iterator start, uint32_t size);
  virtual void Print (std::ostream &os) const;

private:
  uint16_t m_keepAliveTime;
  bool     m_downstreamOnDemand;
  bool     m_loopDetection;
  uint8_t  m_pathVectorLimit;
  uint16_t m_maxPduLength;
  uint32_t m_ldpIdentifier;
};

} // namespace ldp
} // namespace ns3

#endif /* COMMON_SESSION_PARAMS_TLV_H */
