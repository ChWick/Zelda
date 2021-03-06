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

#ifndef _ACTION_TYPES_HPP_
#define _ACTION_TYPES_HPP_

#include "../../../Util/EnumIdMap.hpp"

namespace events {


enum EActionTypes {
  ACTION_MESSAGE,
  ACTION_CREATE_OBJECT,
  ACTION_DELETE_OBJECT,
  ACTION_START_SCRIPT,
};

class CActionTypesMap : public CEnumIdMap<EActionTypes> {
public:
  CActionTypesMap();
};

extern CActionTypesMap ACTION_TYPES_MAP;
};

#endif // _ACTION_TYPES_HPP_
