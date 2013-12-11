#include "StdAfx.h"
#include "HUD.h"
#include "PlayerData.h"
#include "PlayerTool.h"

template<> CHUD* Ogre::Singleton<CHUD>::msSingleton = 0;

const Ogre::Real LETTERS_INTERVALL = 0.05f;
const unsigned int LINES_PER_PAGE = 3;
const unsigned int CHARACTERS_PER_LINE = 20;

CHUD* CHUD::getSingletonPtr(void)
{
    return msSingleton;
}
CHUD& CHUD::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

CHUD::CHUD(CEGUI::Window *guiRoot) :
	m_pHudRoot(0), m_uiCurrentMaxHP(0), m_CurRubyCount(0), m_DesRubyCount(0), m_fRubyTimer(0),m_eTextDisplayState(TDS_NOTHING), m_fLetterTimer(0) {

	CEGUI::ImageManager::getSingleton().loadImageset("hud.imageset");

	m_pHudRoot = guiRoot->createChild("DefaultWindow", "HudRoot");
	m_pHudRoot->setInheritsAlpha(false);
	m_pHudRoot->setAlpha(1);


	m_pMana = m_pHudRoot->createChild("TaharezLook/StaticImage","Mana");
	m_pMana->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0),CEGUI::UDim(0.05f, 0)));
	m_pMana->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,96)));
	m_pMana->setProperty("Image", "hud/Mana");
	m_pMana->setProperty("FrameEnabled","False");
	m_pMana->setProperty("BackgroundEnabled","False");

	m_pManaIndicator = m_pHudRoot->createChild("TaharezLook/StaticImage","ManaIndicator");
	m_pManaIndicator->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 7),CEGUI::UDim(0.05f, 12)));
	m_pManaIndicator->setSize(CEGUI::USize(CEGUI::UDim(0,18),CEGUI::UDim(0,76)));
	m_pManaIndicator->setProperty("Image", "hud/Black");
	m_pManaIndicator->setProperty("FrameEnabled","False");
	m_pManaIndicator->setProperty("BackgroundEnabled","False");

	m_pSelectedItem = m_pHudRoot->createChild("TaharezLook/StaticImage", "SelectedItem");
    m_pSelectedItem->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 32 + 10), CEGUI::UDim(0.05f, 0)));
    m_pSelectedItem->setSize(CEGUI::USize(CEGUI::UDim(0,48), CEGUI::UDim(0,48)));
    m_pSelectedItem->setProperty("Image", "hud/Sword");

	// 2 rows
	for (Ogre::uint16 i = 0; i < m_uiMaxHeartCount; i++) {
		CEGUI::Window *myImageWindow = m_pHudRoot->createChild("TaharezLook/StaticImage","hud" + CEGUI::PropertyHelper<int>::toString(i) );
		myImageWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, (i % (m_uiMaxHeartCount / 2)) * 40.0f + 95.0f),CEGUI::UDim(0.05f, (i / (m_uiMaxHeartCount / 2))* 40.0f)));
		myImageWindow->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
		myImageWindow->setProperty("Image","hud/0/4");
		myImageWindow->setProperty("FrameEnabled","False");
		myImageWindow->setProperty("BackgroundEnabled","False");
		m_pHearts[i] = myImageWindow;
		myImageWindow->setVisible(false);
	}

    CEGUI::Window *pBombs = m_pHudRoot->createChild("TaharezLook/StaticImage","Bombs");
	pBombs->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(4 * 32 + 3 * 4)),CEGUI::UDim(0.05f, 0)));
	pBombs->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	pBombs->setProperty("Image","hud/Bomb");
	pBombs->setProperty("FrameEnabled","False");
	pBombs->setProperty("BackgroundEnabled","False");

	m_pBombCounter = m_pHudRoot->createChild("TaharezLook/StaticText","BombCounter");
	m_pBombCounter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(4 * 32 + 3 * 4)),CEGUI::UDim(0.05f, 32)));
	m_pBombCounter->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	m_pBombCounter->setText("0");
	m_pBombCounter->setProperty("HorzFormatting", "RightAligned");
	m_pBombCounter->setProperty("FrameEnabled","False");
	m_pBombCounter->setProperty("BackgroundEnabled","False");

    CEGUI::Window *pArrows = m_pHudRoot->createChild("TaharezLook/StaticImage","Arrows");
	pArrows->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(3 * 32 + 2 * 4)),CEGUI::UDim(0.05f, 0)));
	pArrows->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	pArrows->setProperty("Image","hud/Arrows");
	pArrows->setProperty("FrameEnabled","False");
	pArrows->setProperty("BackgroundEnabled","False");

	m_pArrowCounter = m_pHudRoot->createChild("TaharezLook/StaticText","ArrowCounter");
	m_pArrowCounter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(3 * 32 + 2 * 4)),CEGUI::UDim(0.05f, 32)));
	m_pArrowCounter->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	m_pArrowCounter->setText("0");
	m_pArrowCounter->setProperty("HorzFormatting", "RightAligned");
	m_pArrowCounter->setProperty("FrameEnabled","False");
	m_pArrowCounter->setProperty("BackgroundEnabled","False");

	CEGUI::Window *pKey = m_pHudRoot->createChild("TaharezLook/StaticImage","Key");
	pKey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(2 * 32 + 1 * 4)),CEGUI::UDim(0.05f, 0)));
	pKey->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	pKey->setProperty("Image","hud/Key");
	pKey->setProperty("FrameEnabled","False");
	pKey->setProperty("BackgroundEnabled","False");

	m_pKeyCounter = m_pHudRoot->createChild("TaharezLook/StaticText","KeyCounter");
	m_pKeyCounter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(2 * 32 + 1 * 4)),CEGUI::UDim(0.05f, 32)));
	m_pKeyCounter->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	m_pKeyCounter->setText("0");
	m_pKeyCounter->setProperty("HorzFormatting", "RightAligned");
	m_pKeyCounter->setProperty("FrameEnabled","False");
	m_pKeyCounter->setProperty("BackgroundEnabled","False");

	CEGUI::Window *pRuby = m_pHudRoot->createChild("TaharezLook/StaticImage","Ruby");
	pRuby->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(1 * 32)),CEGUI::UDim(0.05f, 0)));
	pRuby->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	pRuby->setProperty("Image","hud/Ruby");
	pRuby->setProperty("FrameEnabled","False");
	pRuby->setProperty("BackgroundEnabled","False");

	m_pRubyCounter = m_pHudRoot->createChild("TaharezLook/StaticText","RubyCounter");
	m_pRubyCounter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.95f, -(1 * 32)),CEGUI::UDim(0.05f, 32)));
	m_pRubyCounter->setSize(CEGUI::USize(CEGUI::UDim(0,32),CEGUI::UDim(0,32)));
	m_pRubyCounter->setText("0");
	m_pRubyCounter->setProperty("HorzFormatting", "RightAligned");
	m_pRubyCounter->setProperty("FrameEnabled","False");
	m_pRubyCounter->setProperty("BackgroundEnabled","False");


	m_pTextWindow = m_pHudRoot->createChild("TaharezLook/StaticText", "TextWindow");
	m_pTextWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.95f, -200)));
	m_pTextWindow->setSize(CEGUI::USize(CEGUI::UDim(0.90f,0),CEGUI::UDim(0,200)));
	m_pTextWindow->setProperty("VertFormatting", "TopAligned");
	m_pTextWindow->setVisible(false);
	m_eTextDisplayState = TDS_NOTHING;

	CInputListenerManager::getSingleton().addInputListener(this);
}
void CHUD::start() {
	m_pHudRoot->setVisible(true);
	setMaxHp(CPlayerData::getSingleton().getMaxHP());
	setHP(CPlayerData::getSingleton().getHP());
	setMP(CPlayerData::getSingleton().getMP());
	setRubyCount(CPlayerData::getSingleton().getRubyCount(), false);
	setKeyCount(CPlayerData::getSingleton().getKeyCount());
    setArrowCount(CPlayerData::getSingleton().getArrowCount());
    setBombCount(CPlayerData::getSingleton().getBombCount());

	// showText("asd sdf asdg   sfdgmr   afg  ad fg \n asdfasdga df asdf\n\n asdfadfasgawojijfa dfa n\n\nfasdfaegf hdifha erjhk\n\n\n\n\n\n\ntest");
}
void CHUD::setMaxHp(Hitpoints hp) {
	m_uiCurrentMaxHP = hp;
}
void CHUD::setHP(Hitpoints hp) {
	int fullHearts = hp / 100;
	int quarterHearts = (hp - fullHearts * 100) / 25;

	for (unsigned int i = 0; i < m_uiMaxHeartCount; i++) {
		Hitpoints hp = i * 100;
		if (hp >= m_uiCurrentMaxHP) {
			m_pHearts[i]->setVisible(false);
		}
		else if (hp < fullHearts * 100) {
			m_pHearts[i]->setVisible(true);
			m_pHearts[i]->setProperty("Image","hud/4/4");
		}
		else if (hp < (fullHearts + 1)* 100) {
			m_pHearts[i]->setVisible(true);
			m_pHearts[i]->setProperty("Image","hud/" + CEGUI::PropertyHelper<int>::toString(quarterHearts) + "/4");
		}
		else {
			m_pHearts[i]->setVisible(true);
			m_pHearts[i]->setProperty("Image","hud/0/4");
		}
	}
}
void CHUD::setMP(Ogre::Real mp) {
	m_pManaIndicator->setSize(CEGUI::USize(m_pManaIndicator->getSize().d_width,CEGUI::UDim(0, 74 * (1.0f - mp))));
}
void CHUD::setRubyCount(int ruby, bool animate) {
	if (animate) {
		m_DesRubyCount = ruby;
	}
	else {
		m_DesRubyCount = m_CurRubyCount = ruby;
		m_pRubyCounter->setText(CEGUI::PropertyHelper<int>::toString(m_CurRubyCount));
	}
}
void CHUD::setKeyCount(int key) {
	m_pKeyCounter->setText(CEGUI::PropertyHelper<int>::toString(key));
}
void CHUD::setArrowCount(int arrows) {
	m_pArrowCounter->setText(CEGUI::PropertyHelper<int>::toString(arrows));
}
void CHUD::setBombCount(int bombs) {
    m_pBombCounter->setText(CEGUI::PropertyHelper<int>::toString(bombs));
}
void CHUD::showText(const Ogre::String &text, bool animate) {
	m_uiCurrentDisplayedLines = 0;
	m_sLeftTextToDisplay = text;
	prepareText(m_sLeftTextToDisplay);
	m_fLetterTimer = 0;
	if (m_sLeftTextToDisplay.size() > 0) {
		startDisplayText();
		if (animate) {
			m_eTextDisplayState = TDS_REVEAL_TEXT;
		}
		else {
			showNextTextPage();
		}
	}
}
 void CHUD::prepareText(CEGUI::String &sText) {
    unsigned int iLineLength = 0;
    int iLastWhiteSpace = -1;
    for (unsigned int i = 0; i < sText.length(); i++) {
        if (sText[i] == '\n') {
            iLineLength = 0;
            iLastWhiteSpace = -1;
        }
        else {
            if (sText[i] == ' ') {
                iLastWhiteSpace = i;
            }

            iLineLength++;

            if (iLineLength > CHARACTERS_PER_LINE) {
                if (iLastWhiteSpace < 0) {
                    // TODO: this has not been tested (oversized lines!)
                    iLastWhiteSpace = i;
                    sText.insert(i, "\n");
                    i += 1;
                    iLineLength = 0;
                }
                else {
                    sText[iLastWhiteSpace] = '\n';
                    iLineLength = i - iLastWhiteSpace;
                }
                iLastWhiteSpace = -1;
            }
        }
    }
 }
void CHUD::update(const Ogre::Real tpf) {
	if (m_DesRubyCount != m_CurRubyCount) {
		m_fRubyTimer += tpf;
		if (m_fRubyTimer > 0.15) {
			m_fRubyTimer = 0;
			if (m_DesRubyCount > m_CurRubyCount) {m_CurRubyCount++;}
			else {m_CurRubyCount--;}
			m_pRubyCounter->setText(CEGUI::PropertyHelper<int>::toString(m_CurRubyCount));
		}
	}

	if (m_eTextDisplayState == TDS_REVEAL_TEXT) {
		m_fLetterTimer += tpf;
		if (m_fLetterTimer > LETTERS_INTERVALL) {
			m_fLetterTimer = 0;
			m_pTextWindow->setText(m_pTextWindow->getText() + m_sLeftTextToDisplay[0]);
			if (m_sLeftTextToDisplay[0] == '\n') {
				m_uiCurrentDisplayedLines++;
			}
			m_sLeftTextToDisplay = m_sLeftTextToDisplay.substr(1); // pop first letter
			if (m_sLeftTextToDisplay.size() == 0) {
				m_eTextDisplayState = TDS_WAIT_FOR_USER_INPUT;
			}
			else if (m_uiCurrentDisplayedLines == LINES_PER_PAGE) {
				m_eTextDisplayState = TDS_WAIT_FOR_USER_INPUT;
			}
		}
	}
	else if (m_eTextDisplayState == TDS_WAIT_FOR_USER_INPUT) {
	}
}
bool CHUD::keyPressed( const OIS::KeyEvent &arg ) {
	if (arg.key == OIS::KC_E) {
		if (m_eTextDisplayState == TDS_WAIT_FOR_USER_INPUT) {
			if (m_sLeftTextToDisplay.size() == 0) {
				endDisplayText();
			}
			else {
				m_pTextWindow->setText("");
				m_eTextDisplayState = TDS_REVEAL_TEXT;
				m_uiCurrentDisplayedLines = 0;
				CEGUI::TextUtils::trimLeadingChars(m_sLeftTextToDisplay, " \n\t");
			}
			return false;
		}
		else if (m_eTextDisplayState == TDS_REVEAL_TEXT) {
			// display page
			showNextTextPage();
			m_eTextDisplayState = TDS_WAIT_FOR_USER_INPUT;
			return false;
		}
	}
	return true;
}
void CHUD::startDisplayText() {
	m_pTextWindow->setVisible(true);
	m_pTextWindow->setText("");
	m_uiCurrentDisplayedLines = 0;
	activateTextBox();

	for (HUDListener *pListener : m_lListeners) {
        pListener->startDisplayText();
	}
}
void CHUD::endDisplayText() {
	m_pTextWindow->setVisible(false);
	deactivateTextBox();
	m_eTextDisplayState = TDS_NOTHING;

	for (HUDListener *pListener : m_lListeners) {
        pListener->endDisplayText();
	}
}
void CHUD::setCurrentTool(const CPlayerTool &tool) {
    m_pSelectedItem->setProperty("Image", tool.getImage());
}
void CHUD::showNextTextPage() {

	int index = m_sLeftTextToDisplay.size();
	for (size_t i = 0; i < m_sLeftTextToDisplay.size(); i++) {
		if (m_uiCurrentDisplayedLines >= LINES_PER_PAGE) {
			index = i;
			break;
		}
		if (m_sLeftTextToDisplay[i] == '\n') {
			m_uiCurrentDisplayedLines++;
		}
	}

	m_pTextWindow->setText(m_pTextWindow->getText() + m_sLeftTextToDisplay.substr(0, index));
	m_sLeftTextToDisplay = m_sLeftTextToDisplay.substr(index);
	CEGUI::TextUtils::trimLeadingChars(m_sLeftTextToDisplay, " \n\t");
	m_eTextDisplayState = TDS_WAIT_FOR_USER_INPUT;
}
