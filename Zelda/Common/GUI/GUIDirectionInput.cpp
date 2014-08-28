#include "../Game.hpp"
#include "GUIDirectionInput.hpp"
#include "../Input/GameInputManager.hpp"

using namespace CEGUI;

CGUIDirectionInput::CGUIDirectionInput(CEntity *pParentEntity,
                                       CEGUI::Window *pParentWindow,
                                       float fPixelSize
                                       )
 : CGUIOverlay("input_direction", pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", "input_direction_root")),
   m_vDotCenter(fPixelSize / 2, pParentWindow->getPixelSize().d_height - fPixelSize / 2),
   m_fMaxRadius(0.25 * fPixelSize) {

  m_pRoot->setSize(USize(UDim(0, fPixelSize), UDim(0, fPixelSize)));
  m_pRoot->setPosition(UVector2(UDim(0, 0), UDim(1, -fPixelSize)));

  m_pDot = m_pRoot->createChild("OgreTray/Label", "Dot");
  m_pDot->setPosition(UVector2(UDim(0.45, 0), UDim(0.45, 0)));
  m_pDot->setSize(USize(UDim(0.1, 0), UDim(0.1, 0)));
  m_pDot->setText("O");
}

CGUIDirectionInput::~CGUIDirectionInput() {
}

void CGUIDirectionInput::update(Ogre::Real tpf) {
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
	processInput(Vector2f(state.X.abs,
					  state.Y.abs));
      }
    }
  }

  OIS::Keyboard *pKeyboard = CGame::getSingleton().getInputContext().mKeyboard;
  if (pKeyboard) {
    if (pKeyboard->isKeyDown(OIS::KC_LEFT)) {
      m_vCurrentDotPosition.d_x = -m_fMaxRadius;
      m_bHitOnce = true;
    }
    else if (pKeyboard->isKeyDown(OIS::KC_RIGHT)) {
      m_vCurrentDotPosition.d_x = +m_fMaxRadius;
      m_bHitOnce = true;
    }
    else {
      m_vCurrentDotPosition.d_x = 0;
    }
    if (pKeyboard->isKeyDown(OIS::KC_UP)) {
      m_vCurrentDotPosition.d_y = -m_fMaxRadius;
      m_bHitOnce = true;
    }
    else if (pKeyboard->isKeyDown(OIS::KC_DOWN)) {
      m_vCurrentDotPosition.d_y = m_fMaxRadius;
      m_bHitOnce = true;
    }
    else {
      m_vCurrentDotPosition.d_y = 0;
    }
  }

  if (!m_bHitOnce) {
    m_vCurrentDotPosition = Vector2f::zero();
  }

  m_pDot->setPosition(UVector2(UDim(0.45, m_vCurrentDotPosition.d_x), UDim(0.45, m_vCurrentDotPosition.d_y)));

  if (m_vCurrentDotPosition.d_x < 0) {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_LEFT,	-m_vCurrentDotPosition.d_x));
  }
  else {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_RIGHT, m_vCurrentDotPosition.d_x));
  }

  if (m_vCurrentDotPosition.d_y < 0) {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_FRONT, -m_vCurrentDotPosition.d_y));
  }
  else {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_REAR, m_vCurrentDotPosition.d_y));
  }
}

void CGUIDirectionInput::processInput(const CEGUI::Vector2f &vPosition) {
  if (m_pRoot->isHit(vPosition)) {
    Vector2f vDir = vPosition - m_vDotCenter;
    float fLength = sqrt(vDir.d_x * vDir.d_x + vDir.d_y * vDir.d_y);
    vDir = vDir / fLength;
    m_vCurrentDotPosition = vDir * std::min<float>(fLength, m_fMaxRadius);
    m_bHitOnce = true;
  }
}