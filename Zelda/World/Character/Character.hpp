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

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../WorldEntity.hpp"
#include "CharacterData.hpp"
#include <LinearMath/btAlignedObjectArray.h>
#include "CharacterAttitude.hpp"
#include "CharacterControllerPhysicsListener.hpp"
#include "../Items/ItemTypes.hpp"

class btRigidBody;
class CMap;
class CCharacterController;
class CCharacterItem;

class btCollisionShape;
class btCharacterControllerInterface;

#define ANIM_FADE_SPEED 10.f   // animation crossfade speed of full weight per second

struct SAnimationProperty {
  bool allowMoving;
};

//! Klasse f&uuml;r einen animierten Charater mit Standardanimationen
/*!
  Elternklassen f&uur: Charakter kann zerst&ouml;rt werden, Charakter kann Schaden verursachen.
*/
class CCharacter : public CWorldEntity, protected CCharacterControllerPhysicsListener {
public:
  enum EDefaultAnimationFlags {
    ANIM_FLAG_NONE			= 0,
    ANIM_FLAG_HAS_IDLE		= 1,
    ANIM_FLAG_HAS_RUN		= 2,
    ANIM_FLAG_HAS_WALK		= 4,
    ANIM_FLAG_HAS_HANDS		= 8,
    ANIM_FLAG_HAS_SLICE_HV	= 16,

    ANIM_FLAG_ALL			= 0xFFFFFF
  };
  //! Animationen die man als Charakter haben kann
  enum EDefaultAnimationIDs {
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_SLICE_HORIZONTAL,
    ANIM_USE_ITEM,
    ANIM_RUN,
    /*ANIM_HANDS_CLOSED,
      ANIM_HANDS_RELAXED,
      ANIM_BOW_SHOT,
      ANIM_JUMP_START,
      ANIM_JUMP_LOOP,
      ANIM_JUMP_END,
      ANIM_LEFT,
      ANIM_WALK_HEAVY,
      ANIM_RUN_HEAVY,
      ANIM_FALL,
      ANIM_WALK_ON_LADDER,
      ANIM_WALK_ON_LADDER_END,*/

    ANIM_COUNT
  };

  static const unsigned int ANIM_NONE = ANIM_COUNT;						//!< ANIM_NONE definition: == m_uiAnimationCount
  enum EFadeStade {
    FADE_NONE = 0,
    FADE_IN = 1,
    FADE_OUT = 2,
  };

  enum ECharacterItemSlots {
    CIS_WEAPON = 0,
    CIS_SHIELD,
    CIS_TOOL,
    
    CIS_COUNT,
  };
protected:
  //! Character data
  const SCharacterData &mCharacterData;
  const ECharacterAttitude mAttitude;	//!< Is this character a friend or an enemy?

  bool m_bMoving;
  Ogre::Entity *m_pBodyEntity;									//!< The entity of the body
  btCharacterControllerInterface *mCCPhysics;		//!< The bullet character controller (Physics)
  CCharacterController *m_pCharacterController; //!< The implemented character controller (K.I. or user)

  btAlignedObjectArray<btCollisionShape*> mCollisionShapes; //!< Collision shapes of the character

  Ogre::Real m_fYaw;

  // Animation data
  const unsigned int m_uiAnimationCount;							//!< number of animations
  Ogre::vector<Ogre::AnimationState*>::type m_Anims;	//!< master animation list
  unsigned int m_uiAnimID;														//!< current animation
  Ogre::vector<char>::type m_FadingStates;            //!< which animations are fading

  Ogre::Real m_fTimer;																//!< Animation timer (how long is the current animation running)
  Ogre::Real m_fAnimSpeed;														//!< Animation speed
private:
  std::shared_ptr<CCharacterItem> mCurrentItems[CIS_COUNT];
  std::vector<SAnimationProperty> mAnimationProperty;
public:

  // getter & setter
  Ogre::Entity *getBodyEntity() const {return m_pBodyEntity;}
  btCharacterControllerInterface *getKinematicCharacterController() const {return mCCPhysics;}
  CCharacterController *getCharacterController() const {return m_pCharacterController;}
  const ECharacterAttitude getAttitude() const {return mAttitude;}
  const Ogre::vector<Ogre::AnimationState*>::type &getAnimations() const {return m_Anims;}
  unsigned int getCurrentAnimationID() const {return m_uiAnimID;}
  const std::vector<SAnimationProperty> &getAnimationProperties() const {return mAnimationProperty;}
  const SAnimationProperty &getCurrentAnimationProperty() const {return mAnimationProperty[m_uiAnimID];}

  // reimplemented
  void setPosition(const Ogre::Vector3 &vPos);
  void setOrientation(const Ogre::Quaternion &vRotation);

protected:
  // all values scaled in world units!
  CCharacter(const std::string &sID,
             CEntity *pParent,
             CMap *pMap,
             const SCharacterData &characterData);
  CCharacter(const tinyxml2::XMLElement *pElem,
             CEntity *pParent,
             CMap *pMap,
             const SCharacterData &characterData);

public:
  virtual ~CCharacter();

  virtual void exit();
  void enterMap(CMap *pMap, const Ogre::Vector3 &vInitPosition);

  bool createDamage(const Ogre::Ray &ray, const CDamage &dmg);

  void animRunStart();
  void animRunEnd();
  void animJumpStart();
  void animJumpLoop();
  void animJumpEnd();
  virtual void animAttack();
  virtual void animAttackEnd();
  void animUseToolStart();
  void animUseToolEnd();

  std::shared_ptr<CCharacterItem> getCurrentItem(ECharacterItemSlots slot) {return mCurrentItems[slot];}

  void useCurrentItem();
  void useCurrentWeapon();

  void setIsMoving(bool isMoving);

  virtual void destroy();

  void update(Ogre::Real fTime);

  
  void setAnimation(unsigned int id, bool reset = false);
  const Ogre::AnimationState *getAnimation(unsigned int id) const;
  virtual unsigned int getAnimationIdFromString(const std::string &id) const;
protected:
  virtual void setupInternal() {};
  virtual void createPhysics() = 0;
  virtual void destroyPhysics() = 0;
  virtual void initBody(Ogre::SceneNode *pParentSceneNode) = 0;
  virtual void setupAnimations() = 0;
protected:
  virtual void preUpdateBoundsCallback(const Ogre::Real fTime) {}
  virtual void preAnimationUpdateCallback(const Ogre::Real fTime) {}
  virtual void updateAnimationsCallback(const Ogre::Real fTime);
  virtual void postUpdateAnimationsCallback(const Ogre::Real fTime) {}
  virtual void useItem(EItemVariantTypes item);
  virtual CCharacterController *createCharacterController() = 0;
  void updateAnimations(const Ogre::Real fTime);

  virtual short getCollisionMask();
  virtual short getCollisionGroup();

  virtual bool isReadyForNewAction();
  void fadeAnimations(const Ogre::Real deltaTime);

  void changeItem(ECharacterItemSlots slot, const std::string &bone, EItemVariantTypes item);

private:
  void constructor_impl();

 protected:
  template <class T>
  T *getCharacterController() {
    ASSERT(m_pCharacterController);
    return dynamic_cast<T*>(m_pCharacterController);
  }
  
  template <class T>
  const T *getCharacterController() const {
    ASSERT(m_pCharacterController);
    return dynamic_cast<const T*>(m_pCharacterController);
  }
};

#endif // _CHARACTER_H_
