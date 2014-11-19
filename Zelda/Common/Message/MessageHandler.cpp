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
#include "../Util/Assert.hpp"

template<> CMessageHandler *Ogre::Singleton<CMessageHandler>::msSingleton = 0;

CMessageHandler *CMessageHandler::getSingletonPtr() {
  return msSingleton;
}
CMessageHandler &CMessageHandler::getSingleton() {
  ASSERT(msSingleton);
  return *msSingleton;
}
void CMessageHandler::process() {
  mInjectorMutex.lock();
  while (m_lInjectorsToRemove.size() > 0) {
    m_lInjectors.remove(m_lInjectorsToRemove.front());
    m_lInjectorsToRemove.pop_front();
  }

  m_lInjectors.splice(m_lInjectors.end(), m_lInjectorsToAdd);


  ASSERT(m_lInjectorsToAdd.size() == 0);
  ASSERT(m_lInjectorsToRemove.size() == 0);

  mInjectorMutex.unlock();

  std::list<std::unique_ptr<SMessageEntry> > l;
  mMutex.lock();
  l.splice(l.end(), m_lMessages);
  mMutex.unlock();

  while (l.size() > 0) {
    const CMessage *pMessage = l.front()->pMessage;
    for (auto pInjector : m_lInjectors) {
      pInjector->sendMessageToAll(*pMessage);
    }

    l.pop_front();
  }
  return;
}

void CMessageHandler::addInjector(CMessageInjector *pInjector) {
  mInjectorMutex.lock();
  m_lInjectorsToAdd.push_back(pInjector);
  mInjectorMutex.unlock();
}

void CMessageHandler::removeInjector(CMessageInjector *pInjector) {
  mInjectorMutex.lock();
  m_lInjectorsToRemove.push_back(pInjector);
  mInjectorMutex.unlock();
}

void CMessageHandler::addMessage(const CMessage *m, bool bAutoDelete) {
  mMutex.lock();
  m_lMessages.push_back(std::unique_ptr<SMessageEntry>(new SMessageEntry()));
  m_lMessages.back()->bAutoDelete = bAutoDelete;
  m_lMessages.back()->pMessage = m;
  mMutex.unlock();
}

