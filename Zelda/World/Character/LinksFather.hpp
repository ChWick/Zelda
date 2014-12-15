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

#ifndef _LINKS_FATHER_HPP_
#define _LINKS_FATHER_HPP_

#include "Person.hpp"

class CLinksFather : public CPerson {
public:
  enum ELinksFatherAnimations {
    LF_ANIM_SIT,
    LF_ANIM_STAND_UP,
    LF_ANIM_WALK,
    
    LF_ANIM_COUNT,
  };
  
 private:
 public:
  CLinksFather(const tinyxml2::XMLElement *pElem, CEntity *pParent, CMap *pMap);
  
 private:
  void setupAnimations();
  
  virtual CCharacterController *createCharacterController();
};
#endif // _LINKS_FATHER_HPP_
