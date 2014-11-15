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

#ifndef _ENTRANCE_HPP_
#define _ENTRANCE_HPP_

#include "Region.hpp"

class CEntrance : public CRegion {
private:
  const Ogre::Vector3 mPlayerDirection;
  const Ogre::Vector3 mPlayerRelativePosition;
public:
  CEntrance(CWorldEntity *pParent, const tinyxml2::XMLElement *pElem);

  const Ogre::Vector3 &getPlayerDirection() const {return mPlayerDirection;}
  const Ogre::Vector3 &getPlayerRelativePosition() const {return mPlayerRelativePosition;}
  const Ogre::Vector3 getPlayerAbsolutePosition() const {return mPlayerRelativePosition + getPosition() + mPlayerDirection * getSize();}
};

#endif
