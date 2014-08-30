/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "Event.hpp"
#include "OgreException.h"
#include "../../Log.hpp"
#include "OgreStringConverter.h"
#include "Emitter/EmitterCreator.hpp"
#include "Emitter/Emitter.hpp"
#include "Actions/ActionCreator.hpp"
#include "Actions/Action.hpp"
#include "../Entity.hpp"

using namespace XMLHelper;

namespace events {
CEvent::CEvent(CEntity &owner, const ERepeatTypes eRepeatType, Ogre::Real fRepeatTime)
  : m_sID("unset id"),
    m_eRepeatType(eRepeatType),
    m_fRepeatTime(fRepeatTime),
    m_fTimer(0),
    m_Owner(owner),
    m_bStarted(false) {
}
CEvent::CEvent(CEntity &owner, const tinyxml2::XMLElement *pElement)
  : m_sID(Attribute(pElement, "id", "unset id")),
    m_eRepeatType(REPEAT_TYPES_MAP.parseString(Attribute(pElement, "repeat", "none"))),
    m_fRepeatTime(RealAttribute(pElement, "time", 0)),
    m_fTimer(0),
    m_Owner(owner),
    m_bStarted(BoolAttribute(pElement, "started", false)) {

  for (const tinyxml2::XMLElement *pChildElem = pElement->FirstChildElement(); pChildElem; pChildElem = pChildElem->NextSiblingElement()) {
    if (strcmp(pChildElem->Value(), "emitter") == 0) {
      m_lEmitter.push_back(createEmitter(pChildElem, *this));
    }
    else if (strcmp(pChildElem->Value(), "action") == 0) {
      m_lActions.push_back(createAction(pChildElem, *this));
    }
    else {
      throw Ogre::Exception(0, "Unknown event child '" + std::string(pChildElem->Value()) + "'", __FILE__);
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
    LOGV("Starting event");
    m_bStarted = true;

    for (CAction *pAction : m_lActions) {
      pAction->start();
    }

    switch (m_eRepeatType) {
    case REPEAT_NONE:
      // delete event
      m_Owner.destroyEvent(this);
      break;
    case REPEAT_INFINITE:
      stop();
      break;
    case REPEAT_TIMED:
      m_fTimer = m_fRepeatTime;
      break;
    }
  }
}
void CEvent::stop() {
  if (m_bStarted) {
    LOGV("Stopping event");
    m_bStarted = false;

    for (CAction *pAction : m_lActions) {
      pAction->stop();
    }
  }
}

void CEvent::update(float tpf) {
  if (m_bStarted) {
    if (m_eRepeatType == REPEAT_TIMED) {
      m_fTimer -= tpf;
      if (m_fTimer <= 0) {
        stop();
      }
    }
    return;
  }

  /*for (CEmitter *pEmitter : m_lEmitter) {
    if (pEmitter->isFiring(tpf)) {
      start();
    }
  }*/
}

void CEvent::writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const {
  SetAttribute(pElement, "id", m_sID);
  if (eStyle == OS_FULL) {
    SetAttribute(pElement, "started", m_bStarted);
  }
}

};
