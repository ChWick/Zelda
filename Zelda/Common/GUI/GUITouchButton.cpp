#include "../Game.hpp"
#include "GUITouchButton.hpp"

using namespace CEGUI;

CGUITouchButton::CGUITouchButton(const std::string &id, CEntity *pParentEntity,
                                       CEGUI::Window *pParentWindow,
                                       CEGUI::Window *pRootWindow
                                       )
 : CGUIOverlay(id, pParentEntity, pParentWindow, pRootWindow),
   m_bHitOnce(false) {
}

CGUITouchButton::~CGUITouchButton() {
}

void CGUITouchButton::update(Ogre::Real tpf) {
  m_bHitOnce = false;

  // update input
  OIS::Mouse *pMouse = CGame::getSingleton().getInputContext().mMouse;
  if (pMouse) {
    processInput(Vector2f(pMouse->getMouseState().X.abs,
				      pMouse->getMouseState().Y.abs));
  }

  OIS::MultiTouch *pMultiTouch = CGame::getSingleton().getInputContext().mMultiTouch;
  if (pMultiTouch) {
    const std::vector<OIS::MultiTouchState> &mts = pMultiTouch->getMultiTouchStates();
    for (auto &state : mts) {
      if (state.touchType == OIS::MT_Moved || state.touchType == OIS::MT_Pressed) {
        processInput(Vector2f(state.X.abs, state.Y.abs));
      }
    }
  }


  handleButtonPress(m_bHitOnce);
}

void CGUITouchButton::processInput(const CEGUI::Vector2f &vPosition) {
  if (m_pRoot->isHit(vPosition)) {
    m_bHitOnce = true;
  }
}