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

#include "MessageTypes.hpp"
#include "../Config/TypeDefines.hpp"
#include MESSAGE_USER_TYPES_HEADER

template <class USER_TYPE>
void CMessageTypesMap<USER_TYPE>::init() {
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_SWITCH_MAP] = "switch_map";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_TARGET_REACHED] = "target_reached";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_DEBUG] = "debug";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_ENTITY_STATE_CHANGED] = "entity_type_changed";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_PLAYER_PICKUP_ITEM] = "player_pickup_item";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_HITPOINTS_CHANGED] = "hitpoints_changed";
  CMessageTypesMap<USER_TYPE>::m_Map[MSG_ENTITY_DATA_CHANGED] = "entity_data_changed";
}

template class CMessageTypesMap<MESSAGE_USER_TYPES>;