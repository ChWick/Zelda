#pragma once

#include "OgreSingleton.h"

class CGUIManager : public Ogre::Singleton<CGUIManager> {
public:
    static CGUIManager& getSingleton(void);
    static CGUIManager* getSingletonPtr(void);

	CGUIManager();
	~CGUIManager();

	void update(const Ogre::Real tpf);
};
