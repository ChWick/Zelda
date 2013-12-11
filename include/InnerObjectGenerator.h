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

#ifndef INNEROBJECTGENERATOR_H
#define INNEROBJECTGENERATOR_H

#include "StdAfx.h"

class CMap;
class CPickableObject;

typedef std::pair<float, unsigned int> inner_object_probablity_pair;
typedef std::vector<inner_object_probablity_pair> inner_object_list_type;

class InnerObjectGenerator {
private:
public:
	static inner_object_list_type IOLT_DEFAULT;
	static inner_object_list_type IOLT_EMPTY;
	static std::vector<inner_object_list_type> IOLT_SINGLE_ITEM;


	static void initialize();
	static const inner_object_list_type &getInnerObjectByName(const Ogre::String &name);


	//! This mehtod will check if an object of the given list will be created
	static CPickableObject *createInnerObject(CMap &map, const Ogre::Vector3 &vPosition, const inner_object_list_type &innerObjectList = IOLT_EMPTY);

	//! This method will create a new object
	static CPickableObject *createInnerObject(CMap &map, const Ogre::Vector3 &vPosition, unsigned int innerObject);
};

#endif // INNEROBJECTGENERATOR_H
