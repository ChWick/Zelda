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

#include "StdAfx.h"
#include "InnerObjectGenerator.h"
#include "PickableObject.h"
#include "Map.h"


inner_object_list_type InnerObjectGenerator::IOLT_DEFAULT;
inner_object_list_type InnerObjectGenerator::IOLT_EMPTY;
std::vector<inner_object_list_type> InnerObjectGenerator::IOLT_SINGLE_ITEM;


void InnerObjectGenerator::initialize() {
	if (IOLT_DEFAULT.size() == 0) {
		// not initialised, do it now, care we have to use a cumulative sum!
		float cumsum = 0.f;
		IOLT_DEFAULT.push_back(inner_object_probablity_pair(cumsum += 0.20f, CPickableObject::POT_GREEN_RUBY));			// 20% chanche on green ruby
		IOLT_DEFAULT.push_back(inner_object_probablity_pair(cumsum += 0.05f, CPickableObject::POT_HEART));				// 5% chance on heart
		IOLT_DEFAULT.push_back(inner_object_probablity_pair(cumsum += 0.01f, CPickableObject::POT_BLUE_RUBY));			// 1% chance on blue ruby

		assert(cumsum <= 1.0f);

		// here we initialise the single items that really are in this object (probability > 100%)
		IOLT_SINGLE_ITEM.resize(CPickableObject::POT_COUNT);
		for (size_t i = 0; i < CPickableObject::POT_COUNT; i++) {
			IOLT_SINGLE_ITEM[i].push_back(inner_object_probablity_pair(9999.f, i));
		}
	}
}
const inner_object_list_type &InnerObjectGenerator::getInnerObjectByName(const Ogre::String &name) {
	initialize();

	auto poType = CPickableObject::parsePickableObjectType(name);

	Ogre::String lcName(name);
	Ogre::StringUtil::toLowerCase(lcName);
	if (poType == CPickableObject::POT_COUNT) {
		if (lcName == "default") {
			return IOLT_DEFAULT;
		}
	}
	else if (poType == CPickableObject::POT_EMPTY) {
		return IOLT_EMPTY;
	}
	else {
		return IOLT_SINGLE_ITEM[poType];
	}

	throw Ogre::Exception(__LINE__, name + " is an unknown inner object!", __FILE__);
}
CPickableObject *InnerObjectGenerator::createInnerObject(CMap &map, const Ogre::Vector3 &vPosition, const inner_object_list_type &innerObjectList) {
	initialize();

	// take a random number an compare it to the cum sums
	float rnd = Ogre::Math::UnitRandom();
	unsigned int innerObjectToCreate = CPickableObject::POT_EMPTY;
	for (auto &ioPair : innerObjectList) {
		if (rnd < ioPair.first) {
			// create this object
			innerObjectToCreate = ioPair.second;
			break;
		}
	}

	// call other method to create the object (innerObjectToCreate may be POT_EMPTY)
	return createInnerObject(map, vPosition, innerObjectToCreate);
}
CPickableObject *InnerObjectGenerator::createInnerObject(CMap &map, const Ogre::Vector3 &vPosition, unsigned int innerObject) {
	// check if inner object is valid for creating
	if (innerObject != CPickableObject::POT_EMPTY) {
		CPickableObject *pObject = new CPickableObject(map.getRootSceneNode(), map.getObjectManager(), static_cast<CPickableObject::EPickableObjectTypes>(innerObject), vPosition);
		return pObject;
	}
	return NULL;
}

