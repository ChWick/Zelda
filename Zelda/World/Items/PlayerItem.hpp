#ifndef _PLAYER_ITEM_HPP_
#define _PLAYER_ITEM_HPP_

#include "ItemTypes.hpp"
#include <OgrePrerequisites.h>
#include "../Damage.hpp"

class CCharacter;

class CCharacterItem {
protected:
  CCharacter &mCharacter;
  const EItemVariantTypes mVariantType;
  Ogre::Entity *mAttachedMesh;
public:
  CCharacterItem(CCharacter &character, const std::string &boneToAttach, EItemVariantTypes type);
  ~CCharacterItem();

  EItemVariantTypes getItemVariantType() const {return mVariantType;}

  void show();
  void hide();

  virtual CDamage createDamage();
  virtual Ogre::Vector3 getDamagePosition();
};

#endif // _PLAYER_ITEM_HPP_
