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

#include "mpls.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Mpls);

const uint16_t Mpls::PROT_NUMBER = 0x8847;

TypeId
Mpls::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Mpls")
    .SetParent<Object> ()
  ;
  return tid;
}

Mpls::Mpls ()
  : m_newInterfaceNotification (false)
{
}

Mpls::~Mpls ()
{
}

void
Mpls::EnableNewInterfaceNotification (bool enabled)
{
  m_newInterfaceNotification = enabled;
}

bool
Mpls::IsNewInterfaceNotificationEnabled (void) const
{
  return m_newInterfaceNotification;
}

} // namespace ns3