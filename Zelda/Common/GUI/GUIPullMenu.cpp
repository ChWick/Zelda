#include "GUIPullMenu.hpp"
#include "../Input/InputDefines.hpp"

using namespace CEGUI;

CGUIPullMenu::CGUIPullMenu(const std::string &id,
                           CEntity *pParentEntity,
                           CEGUI::Window *pParentWindow,
                           EPullMenuPositions ePosition,
                           const float &fSize)
  : CGUIOverlay(id, pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", id + "_root")),
    m_ePosition(ePosition),
    m_fSize(fSize),
    m_fDragButtonSize(50),
    m_bPressed(false),
    m_eDragState(DS_SLEEPING),
    m_fDragVelocity(0),
    m_fLastDragPos(0),
    m_fTimeSinceLastTouchMoveEvent(0)
     {
  m_pDragButton = m_pRoot->createChild("OgreTray/StaticImage", "DragButton");
  m_pDragButton->setAlpha(0.5);
  m_pDragButton->setProperty("HorzFormatting", "Tiled");
  m_pDragButton->setProperty("VertFormatting", "CentreAligned");

  m_pDragWindow = m_pRoot->createChild("OgreTray/Group", "DragWindow");
  m_pDragWindow->setText("Drag Window");

  switch (ePosition) {
  case PMD_TOP:
    m_pDragButton->setSize(USize(UDim(1, 0), UDim(0, m_fDragButtonSize)));
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));

    m_pDragWindow->setSize(USize(UDim(1, 0), UDim(0, m_fSize)));
    break;
  }

#ifdef INPUT_TOUCH
  m_pDragButton->
    subscribeEvent(
		   CEGUI::Window::EventMouseButtonDown,
		   Event::Subscriber(&CGUIPullMenu::onDragPressed, this));
  m_pDragButton->
    subscribeEvent(
		   CEGUI::Window::EventMouseEntersArea,
		   Event::Subscriber(&CGUIPullMenu::onDragEnter, this));
  m_pDragButton->
    subscribeEvent(
		   CEGUI::Window::EventMouseButtonUp,
		   Event::Subscriber(&CGUIPullMenu::onDragReleased, this));
  m_pDragButton->
    subscribeEvent(
		   CEGUI::Window::EventMouseMove,
		   Event::Subscriber(&CGUIPullMenu::onDragMoved, this));
  m_pDragButton->
    subscribeEvent(
		   Window::EventMouseLeavesArea,
		   Event::Subscriber(&CGUIPullMenu::onDragMoved, this));
#endif
}


void CGUIPullMenu::update(Ogre::Real tpf) {
  if (m_eDragState == DS_DRAGGING || m_eDragState == DS_OPEN || m_eDragState == DS_OPENING) {
    m_pDragButton->setAlpha(0.5);
    if (m_eDragState == DS_DRAGGING) {
      m_fTimeSinceLastTouchMoveEvent += tpf;
    }
  }
  else {
    m_pDragButton->setAlpha(0);
  }

  if (m_eDragState == DS_CLOSING) {
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
			       UVector2(UDim(0, 0), UDim(0, -tpf * 10 * m_fDragButtonSize)));
  }
  else if (m_eDragState == DS_OPENING) {
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
			       UVector2(UDim(0, 0), UDim(0, tpf * 10 * m_fDragButtonSize)));
  }

  if (m_pDragButton->getPosition().d_y.d_offset > m_pDragWindow->getSize().d_height.d_offset) {
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, m_pDragWindow->getSize().d_height.d_offset)));
    if (m_eDragState == DS_OPENING) {
      m_eDragState = DS_OPEN;
      onOpened();
    }
  }
  else if (m_pDragButton->getPosition().d_y.d_offset < 0) {
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));
    if (m_eDragState == DS_CLOSING) {
      m_eDragState = DS_SLEEPING;
      onClosed();
    }
  }

  m_pDragWindow->setPosition(UVector2(UDim(0, 0), UDim(0, m_pDragButton->getPosition().d_y.d_offset - m_pDragWindow->getSize().d_height.d_offset)));
}


bool CGUIPullMenu::onDragPressed(const CEGUI::EventArgs&) {
  m_bPressed = true;
  onPullStarted();
  m_eDragState = DS_DRAGGING;
  m_fLastDragPos = m_pDragButton->getPosition().d_y.d_offset;
  return true;
}

bool CGUIPullMenu::onDragReleased(const CEGUI::EventArgs&) {
  m_bPressed = false;
  pressReleased();
  return true;
}

bool CGUIPullMenu::onDragMoved(const CEGUI::EventArgs& args) {
  updateDragButtonPosition(args);
  m_fDragVelocity = (m_pDragButton->getPosition().d_y.d_offset - m_fLastDragPos) / m_fTimeSinceLastTouchMoveEvent;
  m_fLastDragPos = m_pDragButton->getPosition().d_y.d_offset;
  m_fTimeSinceLastTouchMoveEvent = 0;
  return true;
}

bool CGUIPullMenu::onDragEnter(const CEGUI::EventArgs& args) {
  if (m_bPressed) {
    onDragPressed(args);
  }
  return true;
}

void CGUIPullMenu::pressReleased() {
  if (m_eDragState != DS_DRAGGING) {
    return;
  }
  //Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(m_fDragVelocity));
  if (m_fDragVelocity < 0) {
    if (m_fDragVelocity < -m_pDragWindow->getSize().d_height.d_offset * 0.05 || m_fLastDragPos < m_pDragWindow->getSize().d_height.d_offset * 0.2) {
      m_eDragState = DS_CLOSING;
      onPullEnded();
    }
    else {
      m_eDragState = DS_OPENING;
    }
  }
  else {
    if (m_fDragVelocity > m_pDragWindow->getSize().d_height.d_offset * 0.05 || m_fLastDragPos > m_pDragWindow->getSize().d_height.d_offset * 0.8) {
      m_eDragState = DS_OPENING;
    }
    else {
      m_eDragState = DS_CLOSING;
      onPullEnded();
    }
  }
}


void CGUIPullMenu::updateDragButtonPosition(const CEGUI::EventArgs& args) {
  const CEGUI::MouseEventArgs &mea = dynamic_cast<const CEGUI::MouseEventArgs&>(args);
  updateDragBar(mea.position.d_y);
}

void CGUIPullMenu::updateDragBar(float fPosY) {
  if (m_eDragState == DS_DRAGGING) {
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, fPosY - 0.25 * m_fDragButtonSize)));
  }
}
