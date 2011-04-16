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

#ifndef LDP_STATUS_CODES_TLV_H
#define LDP_STATUS_CODES_TLV_H

#include <stdint.h>

namespace ns3 {
namespace ldp {

/**
 * \ingroup Ldp
 * Status Codes Summary
 */
class LdpStatusCodes
{
public:
  static const uint32_t BAD_LDP_IDENTIFIER   = 0x80000001;
  static const uint32_t BAD_PROTOCOL_VERSION = 0x80000002;
  static const uint32_t BAD_PDU_LENGTH       = 0x80000003;
  static const uint32_t UNKNOWN_MESSAGE_TYPE = 0x00000004;
  static const uint32_t BAD_MESSAGE_LENGTH   = 0x80000005;
  static const uint32_t UNKNOWN_TLV          = 0x00000006;
  static const uint32_t BAD_TLV_LENGTH       = 0x80000007;
  static const uint32_t MALFORMED_TLV_VALUE  = 0x80000008;
  static const uint32_t HOLD_TIMER_EXPIRED   = 0x80000009;
  static const uint32_t SHUTDOWN             = 0x8000000a;
  static const uint32_t LOOP_DETECTED        = 0x0000000b;
  static const uint32_t UNKNOWN_FEC          = 0x0000000c;
  static const uint32_t NO_ROUTE             = 0x0000000d;
  static const uint32_t NO_LABEL_RESOURCES   = 0x0000000e;
  static const uint32_t LABEL_RESOURCES      = 0x0000000f;
  static const uint32_t SESSION_REJECTED_NO_HELLO = 0x80000010;
  static const uint32_t SESSION_REJECTED_PARAMETER_ADVERTISEMENT_MODE = 0x80000011;
  static const uint32_t SESSION_REJECTED_PARAMETER_MAX_PDU_LENGTH = 0x80000012;
  static const uint32_t SESSION_REJECTED_PARAMETER_LABEL_RANGE = 0x80000013;
  static const uint32_t KEEPALIVE_TIMER_EXPIRED = 0x80000014;
  static const uint32_t LABEL_REQUEST_ABORTED = 0x00000015;
  static const uint32_t MISSING_MESSAGE_PARAMETERS = 0x00000016;
  static const uint32_t UNSUPPORTED_ADDRESS_FAMILY = 0x00000017;
  static const uint32_t SESSION_REJECTED_BAD_KEEPALIVE_TIME = 0x80000018;
  static const uint32_t INTERNAL_ERROR       = 0x80000019;
};

} // namespace ldp
} // namespace ns3

#endif
