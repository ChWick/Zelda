#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "ObjectTypes.hpp"
#include "../WorldEntity.hpp"

class CObject : public CWorldEntity {
protected:
public:
  CObject(const std::string &id, CWorldEntity *pParent, CMap *pMap, EObjectTypes eObjectType);

protected:
  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
};

#endif // _OBJECT_HPP_
