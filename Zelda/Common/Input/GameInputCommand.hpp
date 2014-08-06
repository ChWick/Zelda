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

#ifndef _GAME_INPUT_COMMAND_
#define _GAME_INPUT_COMMAND_

enum EGameInputCommandTypes {
  GIC_RIGHT,
  GIC_LEFT,
  GIC_JUMP,
  GIC_ENTER_LINK,
  GIC_ATTACK,
  GIC_ACTIVATE,
  GIC_USE_HEALTH_POTION,
  GIC_USE_MANA_POTION,
  GIC_CHANGE_WEAPON,
  GIC_SHOW_MENU,

  GIC_COUNT,
};
enum EGameInputState {
  GIS_NONE,
  GIS_PRESSED,
  GIS_HOLD,
  GIS_RELEASED,
  GIS_CLICKED,
};

class CGameInputCommand {
public:
private:
  const EGameInputCommandTypes m_eType;	     //!< Type of the input command
  const float m_fValue;		             //!< Input strength between 0 and 1
  const int m_iValue;                        //!< Integer value, useful in different cases
  const EGameInputState m_eState;	     //!< State of the button

public:
  CGameInputCommand(const EGameInputCommandTypes eType, float fValue = 1, EGameInputState eState = GIS_PRESSED)
    : m_eType(eType), m_fValue(fValue), m_iValue(static_cast<int>(fValue)), m_eState(eState) {
  }
  CGameInputCommand(const EGameInputCommandTypes eType, unsigned int uiValue, EGameInputState eState = GIS_PRESSED)
    : m_eType(eType), m_fValue(uiValue), m_iValue(uiValue), m_eState(eState) {
  }
  CGameInputCommand(const EGameInputCommandTypes eType, int iValue, EGameInputState eState = GIS_PRESSED)
    : m_eType(eType), m_fValue(iValue), m_iValue(iValue), m_eState(eState) {
  }
  CGameInputCommand(const CGameInputCommand &src, EGameInputState eOverrideState)
    : m_eType(src.m_eType), m_fValue(src.m_fValue), m_iValue(src.m_iValue), m_eState(eOverrideState) {
  }

  EGameInputCommandTypes getType() const {return m_eType;}
  float getValue() const {return m_fValue;}
  float getFloatValue() const {return m_fValue;}
  int getIntValue() const {return m_iValue;}
  EGameInputState getState() const {return m_eState;}
  
};

#endif
