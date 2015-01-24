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

#ifndef _TEXT_CONVERTER_HPP_
#define _TEXT_CONVERTER_HPP_

#include <OgreSingleton.h>
#include "Common/Message/MessageInjector.hpp"
#include <CEGUI/String.h>
#include "World/Atlas/MapPack.hpp"

class CAbstractMap;

class CTextConverter : public Ogre::Singleton<CTextConverter>, public CMessageInjector {
protected:
  CAbstractMapPackPtrConst mCurrentMapPack;
public:
  CTextConverter();
  virtual ~CTextConverter();

  static CTextConverter &getSingleton();
  static CTextConverter *getSingletonPtr();

  void convert(CEGUI::String &text);

private:
  virtual void sendMessageToAll(const CMessagePtr);
};

#endif // _TEXT_CONVERTER_HPP_
