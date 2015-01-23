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

#include "ActionTypes.hpp"

namespace events {
void CActionTypesMap::init() {
  m_Map[ACTION_MESSAGE] = "message";
  m_Map[ACTION_CREATE_OBJECT] = "create_object";
  m_Map[ACTION_DELETE_OBJECT] = "delete_object";
  m_Map[ACTION_START_SCRIPT] = "start_script";
  m_Map[ACTION_CREATE_EFFECT] = "create_effect";
}

};
