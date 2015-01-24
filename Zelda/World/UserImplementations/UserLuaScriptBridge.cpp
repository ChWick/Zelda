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
#include <OgreAnimationState.h>
#include <string>

#include "../../Common/Lua/LuaScriptManager.hpp"
#include "../../Common/Log.hpp"
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Util/Sleep.hpp"
#include "../../Common/Message/MessageCreator.hpp"
#include "../../Common/Message/MessageHandler.hpp"
#include "../../Common/Message/MessageInjector.hpp"
#include "../../Common/GameLogic/GameStateManager.hpp"
#include "../../Common/GameLogic/GameStateTypes.hpp"
#include "../../Common/GameLogic/GameLogicGarbageCollector.hpp"
#include "../../Common/Message/MessageTargetReached.hpp"

#include "../../GUIComponents/GUITextBox.hpp"

#include "../WorldEntity.hpp"
#include "../World.hpp"
#include "../Items/ItemTypes.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/Chest.hpp"
#include "../Character/Character.hpp"

using tinyxml2::XMLElement;
using tinyxml2::XMLDocument;

void userRegisterCFunctionsToLua(lua_State *l) {
  registerCFunctionsToLua(l);  // original c bindings

  // user c bindings
  registerSingleCFunctionsToLua(l, entity, "entity");
  registerSingleCFunctionsToLua(l, textMessage, "textMessage");
  registerSingleCFunctionsToLua(l, moveTo, "moveTo");
  registerSingleCFunctionsToLua(l, deleteEntity, "delete");
  registerSingleCFunctionsToLua(l, hasItem, "hasItem");
  registerSingleCFunctionsToLua(l, warp, "warp");
  registerSingleCFunctionsToLua(l, setInnerObject, "setInnerObject");
  registerSingleCFunctionsToLua(l, playAnimation, "playAnimation");
  registerSingleCFunctionsToLua(l, waitForAnimationHasStopped,
                                "waitForAnimationHasStopped");
}


int entity(lua_State *l) {
  LUA_BRIDGE_START(0);

  return numberOfReturnValues;
}

int textMessage(lua_State *l) {
  LUA_BRIDGE_START(1);

  LOGV("Lua call: textMessage");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for textMessage call");
    return numberOfReturnValues;
  }

  std::shared_ptr<CGUITextBox::SResult> result(new CGUITextBox::SResult);
  result->mResult = CGUITextBox::RESULT_NONE;

  XMLDocument doc;
  doc.Parse(lua_tostring(l, 1));

  LOGV("Lua: Creating text message");
  CMessageHandler::getSingleton().addMessage(
      CMessageCreator::getSingleton().createMessage(
          __MSG_LOCATION__,
          doc.FirstChildElement(),
          Ogre::Any(result)));

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
  return 1;  // 1 return value
}



namespace luaHelper {
class CMoveToWait : public CMessageInjector {
 private:
  CEntity *mEntity;
  bool mReached;
  mutable std::mutex mMutex;

 public:
  explicit CMoveToWait(CEntity *pEnt)
      : mEntity(pEnt),
        mReached(false) {
  }

  bool hasReached() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mReached;
  }

 protected:
  void sendMessageToAll(const CMessagePtr m) {
    if (m->getType() == MSG_TARGET_REACHED) {
      auto mtr(
          std::dynamic_pointer_cast<const CMessageTargetReached>(m));
      if (mtr->getEntity() == mEntity) {
        mMutex.lock();
        mReached = true;
        mMutex.unlock();
      }
    }
  }
};

}  // namespace luaHelper


int moveTo(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: moveTo");

  if (lua_gettop(l) < 2) {
    LOGW("Less argument count for moveTo call");
    return 0;
  }

  const std::string id(lua_tostring(l, 1));
  const Ogre::Vector3 position(
      Ogre::StringConverter::parseVector3(lua_tostring(l, 2)));

  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  std::shared_ptr<luaHelper::CMoveToWait> waiter(
      new luaHelper::CMoveToWait(pEntity));
  CGameLogicGarbageCollector::getSingleton().addShared(waiter);

  pEntity->moveToTarget(position);


  while (true) {
    LUA_WAIT(50);
    if (waiter->hasReached()) {
      break;
    }
  }

  return 0;  // 0 return values
}

int deleteEntity(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: deleteEntity");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for moveTo call");
    return 0;
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

int warp(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: warp");

  if (lua_gettop(l) <= 1) {
    LOGW("Wrong argument count for warp call");
    return 0;
  }

  const std::string id(lua_tostring(l, 1));
  const Ogre::Vector3 p(Ogre::StringConverter::parseVector3(lua_tostring(l, 2)));
  Ogre::Quaternion q;
  if (lua_gettop(l) == 4) {
    const Ogre::Degree d(Ogre::StringConverter::parseReal(lua_tostring(l, 3)));
    const Ogre::Vector3 v(Ogre::StringConverter::parseVector3(lua_tostring(l, 4)));
    q = Ogre::Quaternion(d, v);
  }


  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }
  CWorldEntity *pWE (dynamic_cast<CWorldEntity*>(pEntity));
  if (!pEntity) {
    LOGW("Entity '%s' is not a CWorldEntity.", id.c_str());
    return 0;
  }

  pWE->warp(p, q);


  return 0;
}

int hasItem(lua_State *l) {
  LUA_BRIDGE_START(1);

  LOGV("Lua call: hasItem");

  if (lua_gettop(l) != 1) {
    LOGW("Wrong argument count for hasItem call");
    return 1;
  }
  CWorld *pWorld = dynamic_cast<CWorld*>(
      CGameStateManager::getSingleton().getChildRecursive(
          CGameStateIdMap::getSingleton().toString(GST_WORLD)));
  ASSERT(pWorld);

  const std::string item(lua_tostring(l, 1));
  bool has = pWorld->getItemStatusStorage().hasItem(
      CItemVariantIdMap::getSingleton().parseString(item));


  lua_pushboolean(l, has);
  return 1;
}

int setInnerObject(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: setInnerObject");

  if (lua_gettop(l) != 2) {
    LOGW("Wrong argument count for setInnerObject call");
    return 0;
  }

  const std::string id(lua_tostring(l, 1));
  const std::string innerObject(lua_tostring(l, 2));

  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  EObjectTypes innerObjectType(
      CObjectTypeIdMap::getSingleton().parseString(innerObject));

  if (CObject *object = dynamic_cast<CObject *>(pEntity)) {
    object->setInnerObject(innerObjectType);
  } else if (CChest *chest  = dynamic_cast<CChest *>(pEntity)) {
    chest->setInnerObject(innerObjectType);
  } else {
    LOGW("Entity '%s' is not a CObject or a CChest", id.c_str());
  }

  return 0;
}

int playAnimation(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: playAnimation");

  if (lua_gettop(l) > 4 || lua_gettop(l) < 2) {
    LOGW("'%d' is wrong argument count for playAnimations call",
         lua_gettop(l));
  }

  const std::string id(lua_tostring(l, 1));
  const std::string animation(lua_tostring(l, 2));
  bool restart = true;
  bool force = false;

  if (lua_gettop(l) > 2) {
    restart = lua_toboolean(l, 3);
  }
  if (lua_gettop(l) > 3) {
    force = lua_toboolean(l, 4);
  }

  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  // entity has to be a character in order to play animations
  CCharacter *pCharacter(dynamic_cast<CCharacter*>(pEntity));
  if (!pCharacter) {
    LOGW("Entity '%s' is not a CCharacter.", id.c_str());
  }

  // play animation from beginning
  pCharacter->setAnimation(pCharacter->getAnimationIdFromString(animation),
                           restart,
                           force);

  return 0;
}

int waitForAnimationHasStopped(lua_State *l) {
  LUA_BRIDGE_START(0);

  LOGV("Lua call: playAnimation");

  if (lua_gettop(l) != 2) {
    LOGW("'%d' is wrong argument count for playAnimations call",
         lua_gettop(l));
  }

  const std::string id(lua_tostring(l, 1));
  const std::string animation(lua_tostring(l, 2));

  CEntity *pEntity = CGameStateManager::getSingleton().getChildRecursive(id);
  if (!pEntity) {
    LOGW("Entity '%s' was not found in entity tree.", id.c_str());
    return 0;
  }

  // entity has to be a character in order to play animations
  CCharacter *pCharacter(dynamic_cast<CCharacter*>(pEntity));
  if (!pCharacter) {
    LOGW("Entity '%s' is not a CCharacter.", id.c_str());
  }

  const Ogre::AnimationState *pState =
      pCharacter->getAnimation(pCharacter->getAnimationIdFromString(animation));
  if (pState->getLoop()) {
    // looping animation
    Ogre::Real lastTimePosition = 0;
    while (true) {
      LUA_WAIT(50);
      Ogre::Real newTimePosition = pState->getTimePosition();
      if (newTimePosition < lastTimePosition) {
        // we made a loop, so we ended the animation
        break;
      }
      lastTimePosition = newTimePosition;
    }
  } else {
    // non looping animation
    while (true) {
      LUA_WAIT(50);
      if (pState->hasEnded()) {
        break;
      }
    }
  }

  return 0;  // 0 return values
}
