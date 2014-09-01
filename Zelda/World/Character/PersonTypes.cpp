#include "PersonTypes.hpp"  

CPersonTypeIdMap PERSON_TYPE_ID_MAP;


CPersonTypeIdMap::CPersonTypeIdMap() {
  m_Map[PERSON_LINK] = {PERSON_LINK, "link", "link_armor", Ogre::Vector3::UNIT_SCALE * 1.5f, FOE_FRIENDLY};
  m_Map[PERSON_SOLDIER_BLUE] = {PERSON_SOLDIER_BLUE, "soldier", "soldier_blue", Ogre::Vector3::UNIT_SCALE, FOE_ENEMY};
}