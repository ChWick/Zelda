#include "LinksFather.hpp"
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>

CLinksFather::CLinksFather(const tinyxml2::XMLElement *pElem, CEntity *pParent, CMap *pMap)
	: CPerson(pElem, pParent, pMap, LF_ANIM_COUNT) {
}

void CLinksFather::setupAnimations() {
  Ogre::StringVector animNames(LF_ANIM_COUNT);
	animNames[LF_ANIM_SIT] = "Scout";
	animNames[LF_ANIM_WALK] = "Walk";

	// this is very important due to the nature of the exported animations
  m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

  // populate our animation list
  for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
    m_Anims[i] = m_pBodyEntity->getAnimationState(animNames[i]);
    m_Anims[i]->setLoop(true);
    m_FadingStates[i] = FADE_NONE;
    m_Anims[i]->setEnabled(false);
    m_Anims[i]->setWeight(0);
  }


  // start off in the idle state (top and bottom together)
  setAnimation(LF_ANIM_SIT);
}
