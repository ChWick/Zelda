#ifndef _INTERACTION_INTERFACE_HPP_
#define _INTERACTION_INTERFACE_HPP_

#include <OgrePrerequisites.h>

class CWorldEntity;

class CInteractionInterface {
public:
  enum EInteractionDirection {
    ID_NONE  = 0,
    ID_FRONT = 1,
    ID_REAR  = 2,
    ID_LEFT  = 4,
    ID_RIGHT = 8,
    ID_ALL   = ID_FRONT | ID_REAR | ID_LEFT | ID_RIGHT,
  };

  enum EInteractionResult {
    IR_NONE,
    IR_LIFT,
  };

  struct SInteractionResult {
    const EInteractionResult eResult;

    SInteractionResult(EInteractionResult r = IR_NONE)
     : eResult(r) {
    }
    SInteractionResult(SInteractionResult &&) = default;
  };

public:
  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
    return SInteractionResult();
  }
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender) {
    return SInteractionResult();
  }
};

#endif // _INTERACTION_INTERFACE_HPP_
