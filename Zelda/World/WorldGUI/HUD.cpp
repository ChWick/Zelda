#include "HUD.hpp"
#include "../../GUIComponents/GUIHeartsDisplay.hpp"

using namespace CEGUI;

CHUD::CHUD(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("hud", pParentEntity, pParentWindow) {

  m_pRoot = m_pParentWindow->createChild("DefaultWindow", "hud_root");

  Window *m_pLivesText = m_pRoot->createChild("OgreTray/StaticText", "lives_text");
  m_pLivesText->setText("-- LIVES --");
  m_pLivesText->setPosition(UVector2(UDim(0.5, 0), UDim(0, 0)));
  m_pLivesText->setSize(USize(UDim(0.5, 0), UDim(0.05, 0)));
  m_pLivesText->setProperty("FrameEnabled", "false");
  //m_pLivesText->setProperty("TextColor", "1 1 1");

  m_pHeartsDisplay = new CGUIHeartsDisplay(this, m_pRoot);
}
