#include "PersonTypes.hpp"

CPersonTypeIdMap PERSON_TYPE_ID_MAP;
CPersonDataIdMap PERSON_DATA_ID_MAP;


void CPersonTypeIdMap::init() {
  m_Map[PERSON_LINK] = "link
  m_Map[PERSON_SOLDIER_BLOCK] = "soldier_block";
  m_Map[PERSON_SOLDIER_BLUE] = "soldier_blue";
  m_Map[PERSON_SOLDIER_GREEN_SWORD] = "soldier_green_sword";
}

void CPersonDataIdMap::init() {
  m_Map[PERSON_LINK] = {PERSON_LINK, "link", "link_armor", Ogre::Vector3::UNIT_SCALE * 1.5f, FOE_FRIENDLY, HP_ONE_HEART * 3};
  m_Map[PERSON_SOLDIER_BLOCK] = {PERSON_SOLDIER_BLOCK, "soldier", "soldier_green", Ogre::Vector3::UNIT_SCALE, FOE_FRIENDLY, HP_INFINITY};
  m_Map[PERSON_SOLDIER_BLUE] = {PERSON_SOLDIER_BLUE, "soldier", "soldier_blue", Ogre::Vector3::UNIT_SCALE, FOE_ENEMY, HP_HALF_HEART * 3};
  m_Map[PERSON_SOLDIER_GREEN_SWORD] = {PERSON_SOLDIER_GREEN_SWORD, "soldier", "soldier_green", Ogre::Vector3::UNIT_SCALE, FOE_ENEMY, HP_HALF_HEART * 2};
}
