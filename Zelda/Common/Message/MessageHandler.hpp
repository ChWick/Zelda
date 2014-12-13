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

#ifndef _MESSAGE_HANDLER_HPP_
#define _MESSAGE_HANDLER_HPP_

#include <OgreSingleton.h>
#include <list>
#include <mutex>
#include <memory>
#include "Message.hpp"

class CMessageInjector;

class CMessageHandler
  : public Ogre::Singleton<CMessageHandler> {
private:
  mutable std::mutex mMutex;
  mutable std::mutex mInjectorMutex;

  std::list<CMessageInjector*> m_lInjectors;
  std::list<CMessageInjector*> m_lInjectorsToAdd;
  std::list<CMessageInjector*> m_lInjectorsToRemove;
  std::list<MessageEntryType> m_lMessages;
public:
  static CMessageHandler &getSingleton();
  static CMessageHandler *getSingletonPtr();

  void process();

  void addInjector(CMessageInjector *pInjector);
  void removeInjector(CMessageInjector *pInjector);

  void addMessage(CMessagePtr m);
};

#endif
