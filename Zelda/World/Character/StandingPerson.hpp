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

#ifndef _STANDING_PERSON_HPP_
#define _STANDING_PERSON_HPP_

#include "Person.hpp"

class CPlayer;

class CStandingPerson : public CPerson {
public:
  enum EStandingPeronAnimations {
    SP_ANIM_STAND,

    SP_ANIM_COUNT,
  };

private:
public:
	CStandingPerson(const tinyxml2::XMLElement *pElem, CEntity *pParent);

protected:
	void setupInternal();
  void setupAnimations();
  EReceiveDamageResult receiveDamage(const CDamage &dmg);
};

#endif // _STANDING_PERSON_HPP_
