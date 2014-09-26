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

CTextConverter::CTextConverter()
  : mCurrentMap(nullptr) {
}

CTextConverter::~CTextConverter() {
}

void CTextConverter::convert(CEGUI::String &text) {
  ASSERT(mCurrentMap);

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
      toReplace = mCurrentMap->getMapPack()->getLanguageManager().getCEGUIString(id.c_str());
    }

    text = text.replace(replaceStart, replaceEnd - replaceStart + 1, toReplace);
  }

  //
}

void CTextConverter::sendMessageToAll(const CMessage &msg) {
  if (msg.getType() == MSG_SWITCH_MAP) {
    const CMessageSwitchMap &msg_switch_map(dynamic_cast<const CMessageSwitchMap &>(msg));
    if (msg_switch_map.getStatus() == CMessageSwitchMap::FINISHED) {
      mCurrentMap = msg_switch_map.getFromMap();
    }
  }
}
