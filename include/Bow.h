#ifndef _BOW_H_
#define _BOW_H_

#include "StdAfx.h"
#include "Tool.h"

class CBow : public CTool {
public:
    enum EAnims {
        ANIM_BOW_PULL,
        ANIM_BOW_SHOT,

        ANIM_COUNT,
    };
private:
    Ogre::AnimationState *m_Anims[ANIM_COUNT];
    Ogre::Real m_fAnimSpeed;
public:
    CBow(Ogre::SceneManager *pSM)
    : CTool(CPlayerTool::TOOL_BOW, pSM) {

        m_Anims[ANIM_BOW_PULL] = m_pEntity->getAnimationState("BowPull");
        m_Anims[ANIM_BOW_SHOT] = m_pEntity->getAnimationState("BowRelease");

        for (int i = 0; i < ANIM_COUNT; i++) {
            m_Anims[i]->setEnabled(false);
            m_Anims[i]->setLoop(false);
        }

        m_fAnimSpeed = 1;
    }
    void update (Ogre::Real tpf) {
        for (int i = 0; i < ANIM_COUNT; i++) {
            if (m_Anims[i]->getEnabled()) {
                m_Anims[i]->addTime(tpf * m_fAnimSpeed);
            }
        }
    }

    void releaseShot() {
        m_Anims[ANIM_BOW_PULL]->setEnabled(false);
        m_Anims[ANIM_BOW_SHOT]->setEnabled(true);
        m_Anims[ANIM_BOW_SHOT]->setTimePosition(0);

        m_fAnimSpeed = 1;
    }
    void pullString() {
        m_Anims[ANIM_BOW_PULL]->setEnabled(true);
        m_Anims[ANIM_BOW_PULL]->setTimePosition(0);
        m_Anims[ANIM_BOW_SHOT]->setEnabled(false);

        m_fAnimSpeed = 1;
    }
    void releaseString() {
        m_Anims[ANIM_BOW_PULL]->setEnabled(true);
        m_Anims[ANIM_BOW_PULL]->setTimePosition(m_Anims[ANIM_BOW_PULL]->getLength());
        m_Anims[ANIM_BOW_SHOT]->setEnabled(false);

        m_fAnimSpeed = -1;
    }

};

#endif
