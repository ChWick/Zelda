#ifndef _HUD_H_
#define _HUD_H_

#include "StdAfx.h"
#include "Hitpoints.h"
#include "InputListener.h"
#include "GameState.h"
#include "HUDListener.h"

class CPlayerTool;

class CHUD : public Ogre::Singleton<CHUD>, public CInputListener, public CGameStateChanger {
private:
	enum ETextDisplayState {
		TDS_NOTHING,
		TDS_REVEAL_TEXT,
		TDS_WAIT_FOR_USER_INPUT,
	};
	Hitpoints m_uiCurrentMaxHP;
	static const Ogre::uint16 m_uiMaxHeartCount = 20;
	CEGUI::Window *m_pHudRoot;
	CEGUI::Window *m_pHearts[m_uiMaxHeartCount];
	CEGUI::Window *m_pMana;
	CEGUI::Window *m_pManaIndicator;
	CEGUI::Window *m_pSelectedItem;
	CEGUI::Window *m_pBlack;
	CEGUI::Window *m_pArrowCounter;
	CEGUI::Window *m_pKeyCounter;
	CEGUI::Window *m_pRubyCounter;
	CEGUI::Window *m_pBombCounter;
	CEGUI::Window *m_pTextWindow;


	int m_CurRubyCount;
	int m_DesRubyCount;
	Ogre::Real m_fRubyTimer;
	Ogre::Real m_fLetterTimer;					//!< timer for when to display next letter
	unsigned int m_uiCurrentDisplayedLines;		//!< number of lines that are currently displayed
	ETextDisplayState m_eTextDisplayState;		//!< Text display state
	CEGUI::String m_sLeftTextToDisplay;			//!< Text that is left to display
	std::list<HUDListener *> m_lListeners;      //!< listeners
public:
    static CHUD& getSingleton(void);
    static CHUD* getSingletonPtr(void);

	CHUD(CEGUI::Window *guiRoot);

	//! called if the hud shall be shown. It will actualize its values due to the current player data
	void start();

	void addListener(HUDListener *pListener) {m_lListeners.push_back(pListener);}
	void removeListener(HUDListener *pListener) {m_lListeners.remove(pListener);}

	void setMaxHp(Hitpoints hp);
	void setHP(Hitpoints hp);
	void setMP(Ogre::Real mp);

	void setRubyCount(int ruby, bool animate = true);
	void setKeyCount(int key);
	void setArrowCount(int arrows);
	void setBombCount(int bombs);

	void showText(const Ogre::String &text, bool animate = true);

	void setCurrentTool(const CPlayerTool &tool);

	void update(const Ogre::Real tpf);

	bool keyPressed( const OIS::KeyEvent &arg );
private:
	void startDisplayText();
	void endDisplayText();
    void prepareText(CEGUI::String &sText);
	void showNextTextPage();
};

#endif // _HUD_H_
