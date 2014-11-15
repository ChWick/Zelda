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

#include "UserLuaScriptBridge.hpp"
#include <OgreStringConverter.h>
#include "../../Common/Lua/LuaScriptManager.hpp"
#include "../../Common/Log.hpp"
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Util/Sleep.hpp"
#include "../../Common/Message/MessageCreator.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessageInjector.hpp"
#include "../../Common/GameLogic/GameStateManager.hpp"

#include "../../GUIComponents/GUITextBox.hpp"

#include "../../Common/Message/MessageTargetReached.hpp"
#include "../WorldEntity.hpp"

using namespace tinyxml2;

void userRegisterCFunctionsToLua(lua_State *l) {
  registerCFunctionsToLua(l); // original c bindings

  // user c bindings
  registerSingleCFunctionsToLua(l, entity, "entity");
  registerSingleCFunctionsToLua(l, textMessage, "textMessage");
  registerSingleCFunctionsToLua(l, moveTo, "moveTo");
  registerSingleCFunctionsToLua(l, deleteEntity, "delete");
}


int entity(lua_State *l) {
  LUA_BRIDGE_START;

  return 1;
}

int textMessage(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: textMessage");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for textMessage call");
    return -1;
  }

  std::shared_ptr<CGUITextBox::SResult> result(new CGUITextBox::SResult);
  result->mResult = CGUITextBox::RESULT_NONE;

  XMLDocument doc;
  doc.Parse(lua_tostring(l, 1));

  LOGV("Lua: Creating text message");
  CMessageHandler::getSingleton().addMessage(CMessageCreator::getSingleton().createMessage(doc.FirstChildElement(), Ogre::Any(result)));

  LOGV("Lua: Waiting for closing of message box");

  while (true) {
    LUA_WAIT(50);
    result->mMutex.lock();
    if (result->mResult != CGUITextBox::RESULT_NONE) {
      LOGV("Lua: Message box closed");
      break;
    }
    result->mMutex.unlock();
  }

  // still locked
  lua_pushnumber(l, result->mResult);
  result->mMutex.unlock();
  LOGV("Lua: finished");
  return 1; // 1 return value
}



namespace luaHelper {
  class CMoveToWait : public CMessageInjector {
  private:
    CEntity *mEntity;
    bool mReached;
    mutable std::mutex mMutex;
  public:
    CMoveToWait(CEntity *pEnt)
      : mEntity(pEnt),
        mReached(false) {
    }

    bool hasReached() const {std::lock_guard<std::mutex> lock(mMutex); return mReached;}
  protected:
    void sendMessageToAll(const CMessage &m) {
      if (m.getType() == MSG_TARGET_REACHED) {
        const CMessageTargetReached &mtr(dynamic_cast<const CMessageTargetReached&>(m));
        if (mtr.getEntity() == mEntity) {
          mMutex.lock();
          mReached = true;
          mMutex.unlock();
        }
      }
    }
  };
};


int moveTo(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: textMessage");

  if (lua_gettop(l) < 2) {
    LOGW("Less argument count for moveTo call");
    return -1;
  }

  const std::string id(lua_tostring(l, 1));
  const Ogre::Vector3 position(Ogre::StringConverter::parseVector3(lua_tostring(l, 2)));

  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  luaHelper::CMoveToWait waiter(pEntity);

  pEntity->moveToTarget(position);


  while (true) {
    LUA_WAIT(50);
    if (waiter.hasReached()) {
      break;
    }
  }

  return 0; // 0 return values
}

int deleteEntity(lua_State *l) {
  LUA_BRIDGE_START;

  LOGV("Lua call: deleteEntity");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for moveTo call");
    return -1;
  }

  const std::string id(lua_tostring(l, 1));
  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  pEntity->deleteLater();

  return 0;
}

