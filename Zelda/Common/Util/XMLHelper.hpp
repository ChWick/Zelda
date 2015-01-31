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

#ifndef _XML_HELPER_HPP_
#define _XML_HELPER_HPP_

#include "../tinyxml2/tinyxml2.hpp"
#include "EnumIdMap.hpp"
#include "Assert.hpp"
#include <string>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreVector2.h>
#include <OgreException.h>

class CBoundingBox2d;

namespace XMLHelper {
  CBoundingBox2d BoundingBox2dAttribute(const tinyxml2::XMLElement *pElem,
					const Ogre::String &prefix = Ogre::StringUtil::BLANK);
  CBoundingBox2d BoundingBox2dAttribute(const tinyxml2::XMLElement *pElem,
					const CBoundingBox2d &bbDefault,
					const Ogre::String &prefix = Ogre::StringUtil::BLANK);

  bool BoolAttribute(const tinyxml2::XMLElement *pElem,
		     const char *pLabel,
		     bool bDefault = true,
		     bool bRequired = false);

  Ogre::Real RealAttribute(const tinyxml2::XMLElement *pElem,
			   const char *pLabel,
			   Ogre::Real fDefault = 0,
			   bool bRequired = false);

  int IntAttribute(const tinyxml2::XMLElement *pElem,
		   const char *pLabel,
		   int iDefault);


  int IntAttribute(const tinyxml2::XMLElement *pElem,
		   const char *pLabel);

  Ogre::String Attribute(const tinyxml2::XMLElement *pElem,
			 const char *pLabel);

  Ogre::String Attribute(const tinyxml2::XMLElement *pElem,
			 const char *pLabel,
			 const Ogre::String &sDefault);

  Ogre::Vector2 Vector2Attribute(const tinyxml2::XMLElement *pElem,
				 const Ogre::String &prefix = Ogre::StringUtil::BLANK,
				 const Ogre::Vector2 &vDefault = Ogre::Vector2::ZERO,
				 bool bRequired = false);

Ogre::Vector3 Vector3Attribute(const tinyxml2::XMLElement *pElem,
                               const std::string &label,
                               const Ogre::Vector3 &vDefault);

Ogre::Vector3 Vector3Attribute(const tinyxml2::XMLElement *pElem,
                               const std::string &label);

template <class EnumIdMap, class Type>
Type EnumAttribute(const tinyxml2::XMLElement *pElem,
                   const char *pLabel,
                   Type eDefault) {
  ASSERT(pElem);
  ASSERT(pLabel);
  if (!pElem->Attribute(pLabel)) {
    return eDefault;
  }
  return EnumIdMap::getSingleton().parseString(pElem->Attribute(pLabel));
}

template <class EnumIdMap, class Type>
Type EnumAttribute(const tinyxml2::XMLElement *pElem,
                const char *pLabel) {
  ASSERT(pLabel);
  ASSERT(pElem);
  if (!pElem->Attribute(pLabel)) {
    throw Ogre::Exception(0,
                          "Required attribute '" + Ogre::String(pLabel) +
                          "' not found",
                          __FILE__);
  }
  return EnumAttribute<EnumIdMap, Type>(pElem, pLabel);
}


  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    const Ogre::Vector2 &vec);
  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    Ogre::Real value);

  void SetAttribute(tinyxml2::XMLElement *pElem,

		    const CBoundingBox2d &bb,
		    const Ogre::String &prefix = Ogre::StringUtil::BLANK);

  template <class T>
  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    const T &value) {
    pElem->SetAttribute(pLabel, Ogre::StringConverter::toString(value).c_str());
 }

  void SetAttribute(tinyxml2::XMLElement *pElem,
			   const char *pLabel,
			   const char *value);

  template <>
  void SetAttribute<std::string>(tinyxml2::XMLElement *pElem,
				 const char *pLabel,
				 const std::string &value);
  template <>
  void SetAttribute<bool>(tinyxml2::XMLElement *pElem,
				 const char *pLabel,
				 const bool &value);
};





#endif
