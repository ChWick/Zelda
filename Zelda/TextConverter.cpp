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

#include "TextConverter.hpp"
#include "Common/Util/Assert.hpp"
#include "Common/Message/MessageSwitchMap.hpp"
#include "World/Messages/UserMessageTypes.hpp"
#include "World/Atlas/Map.hpp"
#include "World/Atlas/MapPack.hpp"
#include "Common/XMLResources/Manager.hpp"
#include <regex>

template <> CTextConverter *Ogre::Singleton<CTextConverter>::msSingleton = 0;

CTextConverter &CTextConverter::getSingleton() {
  ASSERT(msSingleton);
  return *msSingleton;
}

CTextConverter *CTextConverter::getSingletonPtr() {
  return msSingleton;
}

CTextConverter::CTextConverter() {
}

CTextConverter::~CTextConverter() {
}

void CTextConverter::convert(CEGUI::String &text) {
  LOGI("Converting text");
  ASSERT(mCurrentMapPack);

  int replaceStart = 0;
  while ((replaceStart = text.find("${")) != CEGUI::String::npos) {
    int replaceEnd = text.find("}");
    ASSERT(replaceEnd != CEGUI::String::npos);
    CEGUI::String id(text.substr(replaceStart + 2, replaceEnd - replaceStart - 2));
    ASSERT(id.size() > 0);

    CEGUI::String toReplace;
    if (id == "name") {
      toReplace = "testName";
    }
    else if (id == "button_open" || id == "button_attack" || id == "button_lift" || id == "button_map") {
      toReplace = "unset button";
    }
    else {
      LOGV("Requesting text");
      toReplace = mCurrentMapPack->getLanguageManager().getCEGUIString(id);
      LOGV("Got text");
    }

    text = text.replace(replaceStart, replaceEnd - replaceStart + 1, toReplace);
  }

  LOGI("Converting text finished");
}

void CTextConverter::sendMessageToAll(const CMessagePtr msg) {
  if (msg->getType() == MSG_SWITCH_MAP) {
    auto msg_switch_map(std::dynamic_pointer_cast<const CMessageSwitchMap>(msg));
    if (msg_switch_map->getStatus() == CMessageSwitchMap::FINISHED) {
      mCurrentMapPack = msg_switch_map->getFromMap()->getMapPack();
    }
  }
}
