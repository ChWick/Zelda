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

#include "XMLHelper.hpp"
#include <string>
#include <vector>

namespace XMLHelper {

  bool BoolAttribute(const tinyxml2::XMLElement *pElem,
		     const char *pLabel,
		     bool bDefault,
		     bool bRequired)
    {
    assert(pElem);
    assert(pLabel);
    if (pElem->Attribute(pLabel)) {
      std::string label = pElem->Attribute(pLabel);
      return label == "true" || label == "True" || label == "1";
    }
    else {
      if (bRequired) {
	throw Ogre::Exception(0,
			      "Required attribute '" + Ogre::String(pLabel) + "' not found",
			      __FILE__);
      }
      return bDefault;
    }
  }

  Ogre::Real RealAttribute(const tinyxml2::XMLElement *pElem,
			   const char *pLabel,
			   Ogre::Real fDefault,
			   bool bRequired) {
    if (pElem->Attribute(pLabel)) {
      return pElem->FloatAttribute(pLabel);
    }
    if (bRequired) {
      throw Ogre::Exception(0,
			    "Required attribute '" + Ogre::String(pLabel) + "' not found",
			    __FILE__);
    }
    return fDefault;
  }
  int IntAttribute(const tinyxml2::XMLElement *pElem,
		   const char *pLabel,
		   int iDefault) {
    if (pElem->Attribute(pLabel)) {
      return pElem->IntAttribute(pLabel);
    }
    return iDefault;
  }
  int IntAttribute(const tinyxml2::XMLElement *pElem,
		   const char *pLabel) {
    if (pElem->Attribute(pLabel)) {
      return pElem->IntAttribute(pLabel);
    }
    throw Ogre::Exception(0,
			  "Required attribute '" + Ogre::String(pLabel) + "' not found",
			  __FILE__);
  }

  Ogre::String Attribute(const tinyxml2::XMLElement *pElem,
			 const char *pLabel) {
    assert(pElem);
    assert(pLabel);
    if (pElem->Attribute(pLabel)) {return Ogre::String(pElem->Attribute(pLabel));}

    throw Ogre::Exception(0,
        "Required attribute '" + Ogre::String(pLabel) + "' not found",
        __FILE__);
  }
  Ogre::String Attribute(const tinyxml2::XMLElement *pElem,
			 const char *pLabel,
			 const Ogre::String &sDefault) {
    assert(pElem);
    assert(pLabel);
    if (pElem->Attribute(pLabel)) {return Ogre::String(pElem->Attribute(pLabel));}

    return sDefault;
  }
  Ogre::Vector2 Vector2Attribute(const tinyxml2::XMLElement *pElem,
				 const Ogre::String &prefix,
				 const Ogre::Vector2 &vDefault,
				 bool bRequired) {
    return Ogre::Vector2(RealAttribute(pElem, (prefix + "x").c_str(), vDefault.x, bRequired),
			 RealAttribute(pElem, (prefix + "y").c_str(), vDefault.y, bRequired));
  }

Ogre::Vector3 Vector3Attribute(const tinyxml2::XMLElement *pElem,
                               const std::string &label,
                               const Ogre::Vector3 &vDefault) {
  assert(pElem);

  if (pElem->Attribute(label.c_str())) {
    return Vector3Attribute(pElem, label);
  }

  return vDefault;
}

Ogre::Vector3 Vector3Attribute(const tinyxml2::XMLElement *pElem,
                               const std::string &label) {
  assert(pElem);
  assert(pElem->Attribute(label.c_str()));
  Ogre::String s(pElem->Attribute(label.c_str()));

  Ogre::vector<Ogre::String>::type splitted(Ogre::StringUtil::split(s));

  if (splitted.size() == 1) {
    return Ogre::Vector3(Ogre::StringConverter::parseReal(s));
  } else {
    return Ogre::StringConverter::parseVector3(s);
  }
}


  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    const Ogre::Vector2 &vec) {
    pElem->SetAttribute((Ogre::String(pLabel) + "x").c_str(), vec.x);
    pElem->SetAttribute((Ogre::String(pLabel) + "y").c_str(), vec.y);
  }
  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    Ogre::Real value) {
    pElem->SetAttribute(pLabel, value);
  }

  void SetAttribute(tinyxml2::XMLElement *pElem,
		    const char *pLabel,
		    const char *value) {
    pElem->SetAttribute(pLabel, value);
  }



  template <>
  void SetAttribute<std::string>(tinyxml2::XMLElement *pElem,
				 const char *pLabel,
				 const std::string &value) {
    pElem->SetAttribute(pLabel, value.c_str());
  }

  template <>
  void SetAttribute<bool>(tinyxml2::XMLElement *pElem,
				 const char *pLabel,
				 const bool &value) {
    pElem->SetAttribute(pLabel, value ? "true" : "false");
  }
};
