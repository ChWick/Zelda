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

#include "Entity.hpp"
#include "Events/Event.hpp"
#include "Util/XMLHelper.hpp"
#include "Events/Event.hpp"
#include <OgreStringConverter.h>
#include "Message/Message.hpp"

using namespace events;
using namespace XMLHelper;


CEntity::CEntity(const std::string &sID, unsigned int uiType, CEntity *pParent)
  : m_sID(sID),
    m_uiType(uiType),
    m_bPauseRender(false),
    m_bPauseUpdate(false),
    m_pParent(pParent) {
  attachTo(pParent);
}

CEntity::CEntity(const std::string &sID, CEntity *pParent)
  : m_sID(sID),
    m_uiType(0),
    m_bPauseRender(false),
    m_bPauseUpdate(false),
    m_pParent(pParent) {
  attachTo(pParent);
}
CEntity::CEntity(const CEntity &src)
  : m_sID(src.m_sID),
    m_uiType(src.m_uiType),
    m_bPauseRender(src.m_bPauseRender),
    m_bPauseUpdate(src.m_bPauseUpdate),
    m_pParent(src.m_pParent) {
  attachTo(src.m_pParent);
  // not copying children so far
  // and for events
  // so dont use it...
}
CEntity::CEntity(
		 CEntity *pParent,
		 const tinyxml2::XMLElement *pElem)
  : m_sID(Attribute(pElem, "id", m_sID)),
    m_uiType(IntAttribute(pElem, "type", 0)),
    m_bPauseRender(BoolAttribute(pElem, "pause_render", false)),
    m_bPauseUpdate(BoolAttribute(pElem, "pause_update", false)),
    m_pParent(NULL) {

  attachTo(pParent);

  // read events
  using namespace tinyxml2;
  const XMLElement *pEventsElement = pElem->FirstChildElement("events");
  if (pEventsElement) {
    for (const XMLElement *pEventElement = pEventsElement->FirstChildElement();
          pEventElement;
          pEventElement = pEventElement->NextSiblingElement()) {
      //m_lEvents.push_back(CEventCreator::create(m_Map, *this, pEventElement));
    }
  }
}
CEntity::~CEntity() {
  std::list<CEntity *> lClone(m_lChildren);
  m_lChildren.clear();
  while (lClone.size() > 0) {
    delete lClone.front();
    lClone.pop_front();
  }
  while (m_lEvents.size() > 0) {
    delete m_lEvents.front();
    m_lEvents.pop_front();
  }
  attachTo(NULL);
}
void CEntity::init() {
  for (auto pEvent: m_lEvents) {
    pEvent->init();
  }
  for (std::list<CEvent*>::iterator it = m_lEvents.begin(); it != m_lEvents.end();) {
    /*if ((*it)->getEmitter()->getType() == EventEmitter::EMIT_ON_CREATE) {
      (*it)->start();
      delete (*it);
      it = m_lEvents.erase(it);
    }
    else*/ {
      it++;
    }
  }
}
void CEntity::exit() {
  for (auto pEvent: m_lEvents) {
    pEvent->exit();
  }
}
void CEntity::attachTo(CEntity *pParent) {
  if (m_pParent) {
    m_pParent->m_lChildren.remove(this);
  }
  m_pParent = pParent;

  if (m_pParent) {
    m_pParent->m_lChildren.push_back(this);
  }
}

CEntity *CEntity::getRoot() {
  if (!m_pParent) {return this;}
  return m_pParent->getRoot();
}
const CEntity *CEntity::getRoot() const {
  if (!m_pParent) {return this;}
  return m_pParent->getRoot();
}

CEntity *CEntity::getParent(const std::string &sID) {
  if (!m_pParent) {return NULL;}
  if (m_pParent->m_sID == sID) {
    return m_pParent;
  }
  return m_pParent->getParent();
}

const CEntity *CEntity::getParent(const std::string &sID) const {
  if (!m_pParent) {return NULL;}
  if (m_pParent->m_sID == sID) {
    return m_pParent;
  }
  return m_pParent->getParent();
}

CEntity *CEntity::getChild(const std::string &sID) {
  for (auto *pChild : m_lChildren) {
    if (pChild->m_sID == sID) {
      return pChild;
    }
  }
  return NULL;
}
CEntity *CEntity::getChildRecursive(const std::string &sID) {
  for (auto *pChild : m_lChildren) {
    if (pChild->m_sID == sID) {
      return pChild;
    }
  }
  for (auto *pChild : m_lChildren) {
    CEntity *pRecursiveChild = pChild->getChildRecursive(sID);
    if (pRecursiveChild) {
      return pRecursiveChild;
    }
  }
  return NULL;
}
const CEntity *CEntity::getChild(const std::string &sID) const {
  for (auto *pChild : m_lChildren) {
    if (pChild->m_sID == sID) {
      return pChild;
    }
  }
  return NULL;
}
const CEntity *CEntity::getChildRecursive(const std::string &sID) const {
  for (auto *pChild : m_lChildren) {
    if (pChild->m_sID == sID) {
      return pChild;
    }
  }
  for (auto *pChild : m_lChildren) {
    const CEntity *pRecursiveChild = pChild->getChildRecursive(sID);
    if (pRecursiveChild) {
      return pRecursiveChild;
    }
  }
  return NULL;
}

void CEntity::destroy() {
  for (std::list<CEvent*>::iterator it = m_lEvents.begin(); it != m_lEvents.end();) {
    /*if ((*it)->getEmitter()->getType() == EventEmitter::EMIT_ON_DESTROY) {
      (*it)->start();
      delete (*it);
      it = m_lEvents.erase(it);
    }
    else */{
      it++;
    }
  }
}

void CEntity::sendCallToAll(void (CEntity::*pFunction)()) {
  (this->*pFunction)();
  for (auto pChild : m_lChildren) {
    pChild->sendCallToAll(pFunction);
  }
}

void CEntity::destroyChildren() {
  while (m_lChildren.size() > 0) {
    CEntity *toDel = m_lChildren.front();
    m_lChildren.pop_front();
    delete toDel;
  }
}

void CEntity::destroyEvent(CEvent *pEvent) {
  assert(pEvent);
  m_lEvents.remove(pEvent);
  delete pEvent;
}

void CEntity::sendMessageToAll(const CMessage &message) {
  handleMessage(message);
  for (auto pChild : m_lChildren) {
    pChild->sendMessageToAll(message);
  }
}

void CEntity::update(Ogre::Real tpf) {
  for (auto &pEnt : m_lChildren) {
    if (!pEnt->m_bPauseUpdate) {
      pEnt->update(tpf);
    }
  }
}
void CEntity::render(Ogre::Real tpf) {
  for (auto &pEnt : m_lChildren) {
    if (!pEnt->m_bPauseRender) {
      pEnt->render(tpf);
    }
  }
}

void CEntity::writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const {
  using namespace tinyxml2;

  SetAttribute(pElement, "id", m_sID);
  SetAttribute(pElement, "type", m_uiType);
  if (eStyle == OS_FULL) {
    SetAttribute(pElement, "pause_render", m_bPauseRender);
    SetAttribute(pElement, "pause_update", m_bPauseUpdate);
  }

  // write events
  if (m_lEvents.size() > 0) {
    XMLDocument *pDoc = pElement->GetDocument();
    XMLElement *pEventsElement = pDoc->NewElement("events");
    pElement->InsertEndChild(pEventsElement);

    for (auto pEvent : m_lEvents) {
      XMLElement *pEventElement = pDoc->NewElement("event");
      pEventsElement->InsertEndChild(pEventElement);
      pEvent->writeToXMLElement(pEventElement, eStyle);
    }
  }
}
void CEntity::handleMessage(const CMessage &message) {
}










