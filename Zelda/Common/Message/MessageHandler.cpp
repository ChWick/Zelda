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

#include "MessageHandler.hpp"
#include "MessageInjector.hpp"
#include "Message.hpp"
#include "../Log.hpp"

template<> CMessageHandler *Ogre::Singleton<CMessageHandler>::msSingleton = 0;

CMessageHandler *CMessageHandler::getSingletonPtr() {
  return msSingleton;
}
CMessageHandler &CMessageHandler::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
void CMessageHandler::process() {
  while (m_lMessages.size() > 0) {
    for (auto pInjector : m_lInjectors) {
      pInjector->sendMessageToAll(*m_lMessages.front().pMessage);
    }

    if (m_lMessages.front().bAutoDelete) {
      delete m_lMessages.front().pMessage;
    }
    m_lMessages.pop_front();
  }
}
void CMessageHandler::addMessage(const CMessage *m, bool bAutoDelete) {
  SMessageEntry ent = {
    bAutoDelete,
    m
  };
  m_lMessages.push_back(ent);
}

