/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "Event.hpp"
#include "OgreException.h"
#include "OgreStringConverter.h"

using namespace XMLHelper;

namespace events {

std::string CEvent::toString(ETypes eEventType) {
  switch (eEventType) {
  case EVENT_TO_DEFINE:
    return "event_to_define";
  }

  throw Ogre::Exception(0, "Event type " + Ogre::StringConverter::toString(eEventType) + " could not be converted to a string", __FILE__);
}
CEvent::ETypes CEvent::parseEventType(const std::string &sString) {
  if (sString == "event_to_define") {return EVENT_TO_DEFINE;}

  throw Ogre::Exception(0, "Event type " + sString + " could not be converted to a string", __FILE__);
}



CEvent::CEvent(CEntity &owner, ETypes eType)
  : m_eType(eType),
    m_sID("unset id"),
    m_Owner(owner),
    m_bStarted(false) {
}
CEvent::CEvent(CEntity &owner, ETypes eType, const tinyxml2::XMLElement *pElement)
  : m_eType(eType),
    m_sID(Attribute(pElement, "id", "unset id")),
    m_Owner(owner),
    m_bStarted(BoolAttribute(pElement, "started", false)) {
}
CEvent::~CEvent() {
}
void CEvent::init() {
}
void CEvent::start() {
  if (!m_bStarted) {
    m_bStarted = true;
    start_impl();
  }
}
void CEvent::stop() {
  if (m_bStarted) {
    m_bStarted = false;
    stop_impl();
  }
}
void CEvent::writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const {
  SetAttribute(pElement, "type", toString(m_eType));
  SetAttribute(pElement, "id", m_sID);
  if (eStyle == OS_FULL) {
    SetAttribute(pElement, "started", m_bStarted);
  }
}

};
