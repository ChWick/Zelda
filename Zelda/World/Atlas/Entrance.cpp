#include "Entrance.hpp"
#include "../../Common/Util/XMLHelper.hpp"
#include <OgreStringConverter.h>

using namespace XMLHelper;

CEntrance::CEntrance(CWorldEntity *pParent, const tinyxml2::XMLElement *pElem)
  : CRegion(pParent, pElem),
    mPlayerDirection(Ogre::StringConverter::parseVector3(Attribute(pElem, "player_direction", "0 0 0"))),
    mPlayerRelativePosition(Ogre::StringConverter::parseVector3(Attribute(pElem, "player_relative_position")))
    {
}
