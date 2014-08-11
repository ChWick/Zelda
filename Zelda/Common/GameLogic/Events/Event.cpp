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
#include "EmitterCreator.hpp"
#include "Emitter.hpp"

using namespace XMLHelper;

namespace events {
CEvent::CEvent(CEntity &owner)
  : m_sID("unset id"),
    m_Owner(owner),
    m_bStarted(false) {
}
CEvent::CEvent(CEntity &owner, const tinyxml2::XMLElement *pElement)
  : m_sID(Attribute(pElement, "id", "unset id")),
    m_Owner(owner),
    m_bStarted(BoolAttribute(pElement, "started", false)) {

  for (const tinyxml2::XMLElement *pChildElem = pElement->FirstChildElement(); pChildElem; pChildElem = pChildElem->NextSiblingElement()) {
    if (strcmp(pChildElem->Value(), "emitter") == 0) {
      m_lEmitter.push_back(createEmitter(pChildElem, *this));
    }
  }
}
CEvent::~CEvent() {
  for (CEmitter *pEmitter : m_lEmitter) {
    delete pEmitter;
  }
  m_lEmitter.clear();
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

void CEvent::update(float tpf) {
  if (m_bStarted) {return;}

  for (CEmitter *pEmitter : m_lEmitter) {
    if (pEmitter->isFiring(tpf)) {
      start();
    }
  }
}

void CEvent::writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const {
  SetAttribute(pElement, "id", m_sID);
  if (eStyle == OS_FULL) {
    SetAttribute(pElement, "started", m_bStarted);
  }
}

};
