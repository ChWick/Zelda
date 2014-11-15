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

#include "CharacterCreator.hpp"
#include "../../Common/tinyxml2/tinyxml2.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include "../../Common/Util/Assert.hpp"

#include "../Character/SimpleEnemy.hpp"
#include "../Character/StandingPerson.hpp"
#include "../Character/LinksFather.hpp"

using namespace XMLHelper;

CWorldEntity* CCharacterCreator::createCharacter(const tinyxml2::XMLElement *pElem,
                                                 CWorldEntity *pParent,
                                                 CMap *pMap,
                                                 CWorldEntity *pPlayer) {
  const std::string sEntityType(Attribute(pElem, "entity_type"));

  const Ogre::Real rotation(RealAttribute(pElem, "rotation",0));
  const Ogre::Quaternion qOrientation(Ogre::Degree(rotation), Ogre::Vector3::UNIT_Y);
  const Ogre::Vector3 position(Ogre::StringConverter::parseVector3(Attribute(pElem, "position")));

  CWorldEntity *pEntity(nullptr);

  if (sEntityType == "simple_enemy") {
    CSimpleEnemy *pEnemy = new CSimpleEnemy(pElem, pParent, pMap);
    // special case, set player requires that enemy entered map
    pEnemy->enterMap(pMap, position);
    pEnemy->setOrientation(qOrientation);
    pEnemy->setPlayer(pPlayer);
    return pEnemy;
  }
  else if (sEntityType == "standing_person") {
    pEntity = new CStandingPerson(pElem, pParent, pMap);
  }
  else if (sEntityType == "links_father") {
    pEntity = new CLinksFather(pElem, pParent, pMap);
  }
  else {
    return nullptr; // unknown
  }

  ASSERT(pEntity);

  pEntity->enterMap(pMap, position);
  pEntity->setOrientation(qOrientation);

  return pEntity;
}
