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
