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
