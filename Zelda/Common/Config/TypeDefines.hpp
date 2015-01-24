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

#ifndef _TYPE_DEFINES_HPP_
#define _TYPE_DEFINES_HPP_

// The game class to use
#define GAME_CLASS_HEADER "Zelda.hpp"
#define GAME_CLASS CZelda

// The position type to use (2dim or 3dims)
#define SPATIAL_VECTOR_HEADER <OgreVector3.h>
#define SPATIAL_VECTOR Ogre::Vector3

// The root entity class to use
#define ENTITY_HEADER "Common/GameLogic/Entity.hpp"
#define ENTITY CEntity

// The message creator
#define MESSAGE_CREATOR_HEADER "World/Messages/UserMessageCreator.hpp"
#define MESSAGE_CREATOR CUserMessageCreator

// User message types
#define MESSAGE_USER_TYPES_HEADER "World/Messages/UserMessageTypes.hpp"
#define MESSAGE_USER_TYPES CUserMessageTypesMap

// User lua script bridge
#define LUA_SCRIPT_BRIDGE_HEADER "World/UserImplementations/UserLuaScriptBridge.hpp"
#define LUA_SCRIPT_BRIDGE_REGISTER_FUNCTION(l) userRegisterCFunctionsToLua(l)


#endif // _TYPE_DEFINES_HPP_
