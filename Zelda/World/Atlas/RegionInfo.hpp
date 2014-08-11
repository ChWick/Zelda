#ifndef _REGION_INFO_HPP_
#define _REGION_INFO_HPP_

#include <OgreVector3.h>
#include <string>

struct SRegionInfo {
  Ogre::Vector3 position;
  Ogre::Vector3 size;
  std::string ID;
  std::string shape;
};

#endif // _REGION_INFO_HPP_
