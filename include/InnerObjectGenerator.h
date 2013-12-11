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
