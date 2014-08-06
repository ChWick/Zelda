/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <vector>
#include <string>
#include <OgreException.h>

struct SStatistics;
class CEntity;

class CMessage {
public:
  enum EMessageTypes {
    MT_MESSAGE_UNSET,
    MT_MAP_DESTROYED,
    MT_ENTITY_DESTROYED,
    MT_TOGGLE_TOUCH_INPUT_BLINK,
    MT_MESSAGE_BOX_PAGE_CHANGED,
    MT_EXIT_REACHED,
    MT_DROP_DOWN_MENU,
    MT_WEAPON_CHANGED,
    MT_AT_LEVEL_END,

    MT_SOCIAL_GAMING,
  };
  
  static std::string toString(EMessageTypes eMT) {
    switch (eMT) {
    case MT_MESSAGE_UNSET:
      return "unset";
    case MT_MAP_DESTROYED:
      return "map_destroyed";
    case MT_ENTITY_DESTROYED:
      return "entity_destroyed";
    case MT_TOGGLE_TOUCH_INPUT_BLINK:
      return "toggle_touch_input_blink";
    case MT_MESSAGE_BOX_PAGE_CHANGED:
      return "message_box_page_changed";
    case MT_EXIT_REACHED:
      return "exit_reached";
    case MT_DROP_DOWN_MENU:
      return "drop_down_menu";
    case MT_WEAPON_CHANGED:
      return "weapon_changed";
    case MT_SOCIAL_GAMING:
      return "social_gaming";
    case MT_AT_LEVEL_END:
      return "at_level_end";
    }

    throw Ogre::Exception(0, "Message type could not be converted to string", __FILE__);
  }
  static EMessageTypes parseMessageType(const std::string &s) {
    if (s == "unset") {return MT_MESSAGE_UNSET;}
    else if (s == "map_destroyed") {return MT_MAP_DESTROYED;}
    else if (s == "toggle_touch_input_blink") {return MT_TOGGLE_TOUCH_INPUT_BLINK;}
    else if (s == "message_box_page_changed") {return MT_MESSAGE_BOX_PAGE_CHANGED;}
    else if (s == "exit_reached") {return MT_EXIT_REACHED;}
    else if (s == "entity_destroyed") {return MT_ENTITY_DESTROYED;}
    else if (s == "drop_down_menu") {return MT_DROP_DOWN_MENU;}
    else if (s == "weapon_changed") {return MT_WEAPON_CHANGED;}

    else if (s == "social_gaming") {return MT_SOCIAL_GAMING;}
    else if (s == "at_level_end") {return MT_AT_LEVEL_END;}

    throw Ogre::Exception(0, "Message type could not be parsed: " + s, __FILE__);
  }
  

  static const unsigned int MAX_INT_VALUES = 2;
  static const unsigned int MAX_BOOL_VALUES = 2;
protected:
  unsigned int m_uiType;

  std::string m_sID;
  std::vector<int> m_aIntValues;
  std::vector<bool> m_aBoolValues;
  const CEntity *m_pEntity;
  const SStatistics *m_pStatistics;
public:
  CMessage(const unsigned int uiType = MT_MESSAGE_UNSET)
    : m_uiType(uiType),
      m_aIntValues(MAX_INT_VALUES, 0),
      m_aBoolValues(MAX_BOOL_VALUES, false),
      m_pEntity(NULL),
      m_pStatistics(nullptr) {
  }
  CMessage(const unsigned int uiType, int i0, int i1 = 0)
    : m_uiType(uiType),
      m_aIntValues(MAX_INT_VALUES),
      m_aBoolValues(MAX_BOOL_VALUES, false),
      m_pEntity(NULL),
      m_pStatistics(nullptr) {
    m_aIntValues[0] = i0;
    m_aIntValues[1] = i1;
  }
  CMessage(const CMessage &src) 
    : m_uiType(src.m_uiType),
      m_sID(src.m_sID),
      m_aIntValues(src.m_aIntValues),
      m_aBoolValues(src.m_aBoolValues),
      m_pEntity(src.m_pEntity),
      m_pStatistics(src.m_pStatistics) {
  }
  unsigned int getType() const {return m_uiType;}
  CMessage &setType(unsigned int uiType) {m_uiType = uiType; return *this;}

  int getInt(unsigned int idx = 0) const {return m_aIntValues[idx];}
  CMessage &setInt(int i, unsigned int idx = 0) {m_aIntValues[idx] = i; return *this;}

  bool getBool(unsigned int idx = 0) const {return m_aBoolValues[idx];}
  CMessage &setBool(bool b, unsigned int idx = 0) {m_aBoolValues[idx] = b; return *this;}

  const std::string &getID() const {return m_sID;}
  CMessage &setID(const std::string &sID) {m_sID = sID; return *this;}

  const CEntity *getEntity() const {return m_pEntity;}
  CMessage &setEntity(const CEntity *pEntity) {m_pEntity = pEntity; return *this;}

  const SStatistics *getStatistics() const {return m_pStatistics;}
  CMessage &setStatistics(const SStatistics *pStatistics) {m_pStatistics = pStatistics; return *this;}
};

#endif
