#ifndef _MAP_USER_DATA_PARSER_H_
#define _MAP_USER_DATA_PARSER_H_

#include "StdAfx.h"
#include "PickableObject.h"
#include "InnerObjectGenerator.h"

class CMapUserDataParser {
public:
	struct SChestUserData {
		const Ogre::String _id;
		const inner_object_list_type &_innerObject;

		SChestUserData(const Ogre::String &id, const inner_object_list_type &io)
		: _id(id), _innerObject(io) {
		}
		SChestUserData(const SChestUserData &src)
			: _id(src._id), _innerObject(src._innerObject) {
		}
	};

	struct SSignUserData {
		const Ogre::String _id;
		const Ogre::String _front;
		const Ogre::String _rear;

		SSignUserData(const Ogre::String &id, const Ogre::String &front, const Ogre::String &rear)
		: _id(id), _front(front), _rear(rear) {
		}
		SSignUserData(const SSignUserData &src)
			: _id(src._id), _front(src._front), _rear(src._rear) {
		}
	};
private:
	std::vector<SChestUserData> m_vChestUserData;
	std::vector<SSignUserData> m_vSignUserData;
public:
	CMapUserDataParser() {
	}

	void reset();
	void load(const Ogre::String &file, const Ogre::String &groupName);

	const SChestUserData &getChestUserDataById(const Ogre::String &id);
	const SSignUserData &getSignUserDataById(const Ogre::String &id);

private:
	void parseChests(tinyxml2::XMLElement *pElem);
	void parseChest(tinyxml2::XMLElement *pElem);

	void parseSigns(tinyxml2::XMLElement *pElem);
	void parseSign(tinyxml2::XMLElement *pElem);
};

#endif
