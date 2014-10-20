#ifndef _PLAYER_ITEM_HPP_
#define _PLAYER_ITEM_HPP_

#include "ItemTypes.hpp"
#include <OgrePrerequisites.h>
#include "../Damage.hpp"

class CPlayer;

class CPlayerItem {
protected:
  CPlayer &mPlayer;
  const EItemVariantTypes mVariantType;
  Ogre::Entity *mAttachedMesh;
public:
  CPlayerItem(CPlayer &player, const std::string &boneToAttach, EItemVariantTypes type);
  ~CPlayerItem();

  void show();
  void hide();

  virtual CDamage createDamage();
  virtual Ogre::Vector3 getDamagePosition();
};

#endif // _PLAYER_ITEM_HPP_
