#ifndef _DEBUG_DISPLAY_H_
#define _DEBUG_DISPLAY_H_

#include "StdAfx.h"

class CDebugDisplay : public Ogre::Singleton<CDebugDisplay> {
private:
	OgreBites::ParamsPanel *mDetailsPanel;
	Ogre::list<const Ogre::Real *>::type m_RealData;
public:
	CDebugDisplay(OgreBites::SdkTrayManager *pTrayManager) {
		Ogre::StringVector items;
		items.push_back("Player Map");
		items.push_back("Camera Map");
		items.push_back("f1");
		items.push_back("f2");
		items.push_back("f3");
		items.push_back("f4");
		items.push_back("Strg");

		mDetailsPanel = pTrayManager->createParamsPanel(OgreBites::TL_NONE, "DebugPanel", 600, items);
		mDetailsPanel->hide();
	}
	OgreBites::ParamsPanel *getPanel() {return mDetailsPanel;}
	void update() {
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
			int index = 2;
            for (auto data : m_RealData) {
                mDetailsPanel->setParamValue(index, Ogre::StringConverter::toString(*data));
				index++;
			}
        }
	}
	void setDebugString(const Ogre::String &str, int index = 6) {
		mDetailsPanel->setParamValue(index, str);
	}
	void addRealData(const Ogre::Real &r) {
		m_RealData.push_back(&r);
	}

    static CDebugDisplay& getSingleton(void);
    static CDebugDisplay* getSingletonPtr(void);
};

#endif