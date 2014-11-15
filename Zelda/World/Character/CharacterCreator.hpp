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

#ifndef _CHARACTER_CREATOR_HPP_
#define _CHARACTER_CREATOR_HPP_

class CWorldEntity;
class CMap;

namespace tinyxml2 {
  class XMLElement;
};

class CCharacterCreator {
public:
  //! Creates a character based on a xml element.
  /**
    * @return the newly created world entity or a nullptr if the character type is unknown or an error occurred
    */
  static CWorldEntity* createCharacter(const tinyxml2::XMLElement *pElem, CWorldEntity *pParent, CMap *pMap, CWorldEntity *pPlayer);
};

#endif // _CHARACTER_CREATOR_HPP_
