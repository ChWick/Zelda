#include "GUIPullMenu.hpp"
#include "../Input/InputDefines.hpp"
#include "../Log.hpp"

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
  CInputListenerManager::getSingleton().addInputListener(this);

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
  case PMD_BOTTOM:
    m_pDragButton->setSize(USize(UDim(1, 0), UDim(0, m_fDragButtonSize)));
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, m_pRoot->getPixelSize().d_height - m_fDragButtonSize)));

    m_pDragWindow->setSize(USize(UDim(1, 0), UDim(0, m_fSize)));
    break;
  case PMD_RIGHT:
    m_pDragButton->setSize(USize(UDim(0, m_fDragButtonSize), UDim(1, 0)));
    m_pDragButton->setPosition(UVector2(UDim(0, m_pRoot->getPixelSize().d_width - m_fDragButtonSize), UDim(0, 0)));

    m_pDragWindow->setSize(USize(UDim(0, m_fSize), UDim(1, 0)));
    break;
  case PMD_LEFT:
    m_pDragButton->setSize(USize(UDim(0, m_fDragButtonSize), UDim(1, 0)));
    m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));

    m_pDragWindow->setSize(USize(UDim(0, m_fSize), UDim(1, 0)));
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
		   Event::Subscriber(&CGUIPullMenu::onDragLeft, this));
#endif
}

CGUIPullMenu::~CGUIPullMenu() {
  CInputListenerManager::getSingleton().removeInputListener(this);
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
    translateDragButton(-tpf * 10 * m_fDragButtonSize);
  }
  else if (m_eDragState == DS_OPENING) {
    translateDragButton(tpf * 10 * m_fDragButtonSize);
  }

  switch (m_ePosition) {
  case PMD_TOP:
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
    break;
  case PMD_BOTTOM:
    if (m_pDragButton->getPosition().d_y.d_offset + m_pDragButton->getSize().d_height.d_offset < m_pRoot->getPixelSize().d_height - m_pDragWindow->getSize().d_height.d_offset) {
      m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, m_pRoot->getPixelSize().d_height - m_pDragWindow->getSize().d_height.d_offset - m_pDragButton->getSize().d_height.d_offset)));
      if (m_eDragState == DS_OPENING) {
        m_eDragState = DS_OPEN;
        onOpened();
      }
    }
    else if (m_pDragButton->getPosition().d_y.d_offset + m_pDragButton->getSize().d_height.d_offset > m_pRoot->getPixelSize().d_height) {
      m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, m_pRoot->getPixelSize().d_height - m_pDragButton->getSize().d_height.d_offset)));
      if (m_eDragState == DS_CLOSING) {
        m_eDragState = DS_SLEEPING;
        onClosed();
      }
    }
    m_pDragWindow->setPosition(UVector2(UDim(0, 0), UDim(0, m_pDragButton->getPosition().d_y.d_offset + m_pDragButton->getSize().d_height.d_offset)));
    break;
  case PMD_LEFT:
    if (m_pDragButton->getPosition().d_x.d_offset > m_pDragWindow->getSize().d_width.d_offset) {
      m_pDragButton->setPosition(UVector2(UDim(0, m_pDragWindow->getSize().d_width.d_offset), UDim(0, 0)));
      if (m_eDragState == DS_OPENING) {
        m_eDragState = DS_OPEN;
        onOpened();
      }
    }
    else if (m_pDragButton->getPosition().d_x.d_offset < 0) {
      m_pDragButton->setPosition(UVector2(UDim(0, 0), UDim(0, 0)));
      if (m_eDragState == DS_CLOSING) {
        m_eDragState = DS_SLEEPING;
        onClosed();
      }
    }
    m_pDragWindow->setPosition(UVector2(UDim(0, m_pDragButton->getPosition().d_x.d_offset - m_pDragWindow->getSize().d_width.d_offset), UDim(0, 0)));
    break;
  case PMD_RIGHT:
    if (m_pDragButton->getPosition().d_x.d_offset + m_pDragButton->getSize().d_width.d_offset < m_pRoot->getPixelSize().d_width - m_pDragWindow->getSize().d_width.d_offset) {
      m_pDragButton->setPosition(UVector2(UDim(0, m_pRoot->getPixelSize().d_width - m_pDragWindow->getSize().d_width.d_offset - m_pDragButton->getSize().d_width.d_offset), UDim(0, 0)));
      if (m_eDragState == DS_OPENING) {
        m_eDragState = DS_OPEN;
        onOpened();
      }
    }
    else if (m_pDragButton->getPosition().d_x.d_offset + m_pDragButton->getSize().d_width.d_offset > m_pRoot->getPixelSize().d_width) {
      m_pDragButton->setPosition(UVector2(UDim(0, m_pRoot->getPixelSize().d_width - m_pDragButton->getSize().d_width.d_offset), UDim(0, 0)));
      if (m_eDragState == DS_CLOSING) {
        m_eDragState = DS_SLEEPING;
        onClosed();
      }
    }
    m_pDragWindow->setPosition(UVector2(UDim(0, m_pDragButton->getPosition().d_x.d_offset + m_pDragButton->getSize().d_width.d_offset), UDim(0, 0)));
    break;
  }
}

void CGUIPullMenu::translateDragButton(float fDelta) {
  switch (m_ePosition) {
  case PMD_BOTTOM:
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
			       UVector2(UDim(0, 0), UDim(0, -fDelta)));
    break;
  case PMD_TOP:
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
			       UVector2(UDim(0, 0), UDim(0, fDelta)));
    break;
  case PMD_RIGHT:
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
			       UVector2(UDim(0, -fDelta), UDim(0, 0)));
		break;
  case PMD_LEFT:
    m_pDragButton->setPosition(m_pDragButton->getPosition() +
            UVector2(UDim(0, fDelta), UDim(0, 0)));
    break;
  }
}

bool CGUIPullMenu::onDragPressed(const CEGUI::EventArgs&) {
  m_bPressed = true;
  onPullStarted();
  m_eDragState = DS_DRAGGING;

  switch (m_ePosition) {
  case PMD_TOP:
  case PMD_BOTTOM:
    m_fLastDragPos = m_pDragButton->getPosition().d_y.d_offset;
    break;
  case PMD_LEFT:
  case PMD_RIGHT:
    m_fLastDragPos = m_pDragButton->getPosition().d_x.d_offset;
    break;
  }
  return true;
}

bool CGUIPullMenu::onDragReleased(const CEGUI::EventArgs&) {
  m_bPressed = false;
  pressReleased();
  return true;
}

bool CGUIPullMenu::onDragMoved(const CEGUI::EventArgs& args) {
  updateDragButtonPosition(args);

  switch (m_ePosition) {
  case PMD_TOP:
  case PMD_BOTTOM:
    m_fDragVelocity = (m_pDragButton->getPosition().d_y.d_offset - m_fLastDragPos) / m_fTimeSinceLastTouchMoveEvent;
    m_fLastDragPos = m_pDragButton->getPosition().d_y.d_offset;
    break;
  case PMD_LEFT:
  case PMD_RIGHT:
    m_fDragVelocity = (m_pDragButton->getPosition().d_x.d_offset - m_fLastDragPos) / m_fTimeSinceLastTouchMoveEvent;
    m_fLastDragPos = m_pDragButton->getPosition().d_x.d_offset;
    break;
  }

  m_fTimeSinceLastTouchMoveEvent = 0;
  return true;
}

bool CGUIPullMenu::onDragEnter(const CEGUI::EventArgs& args) {
  if (m_bPressed) { //  mouse/touch has to be pressed
    onDragPressed(args);
  }
  return true;
}

bool CGUIPullMenu::onDragLeft(const CEGUI::EventArgs& args) {
  if (m_eDragState == DS_DRAGGING) {
    // follow mouse position
    onDragMoved(args);
  }

  return true;
}

void CGUIPullMenu::pressReleased() {
  m_bPressed = false;

  if (m_eDragState != DS_DRAGGING) {
    return;
  }
  //Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(m_fDragVelocity));
  switch (m_ePosition) {
  case PMD_TOP:
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
    break;
  case PMD_BOTTOM:
    if (m_fDragVelocity > 0) {
      m_eDragState = DS_CLOSING;
      onPullEnded();
    }
    else {
      m_eDragState = DS_OPENING;
    }
    break;
  case PMD_RIGHT:
    if (m_fDragVelocity > 0) {
      m_eDragState = DS_CLOSING;
      onPullEnded();
    }
    else {
      m_eDragState = DS_OPENING;
    }
    break;
  case PMD_LEFT:
    if (m_fDragVelocity <= 0) {
      m_eDragState = DS_CLOSING;
      onPullEnded();
    }
    else {
      m_eDragState = DS_OPENING;
    }
    break;
  }
}


void CGUIPullMenu::updateDragButtonPosition(const CEGUI::EventArgs& args) {
  const CEGUI::MouseEventArgs &mea = dynamic_cast<const CEGUI::MouseEventArgs&>(args);
  switch (m_ePosition) {
  case PMD_TOP:
  case PMD_BOTTOM:
    updateDragBar(0, mea.position.d_y - 0.25 * m_fDragButtonSize);
    break;
  case PMD_LEFT:
  case PMD_RIGHT:
    updateDragBar(mea.position.d_x - 0.25 * m_fDragButtonSize, 0);
    break;
  }
}

void CGUIPullMenu::updateDragBar(float fPosX, float fPosY) {
  if (m_eDragState == DS_DRAGGING) {
    m_pDragButton->setPosition(UVector2(UDim(0, fPosX), UDim(0, fPosY)));
  }
}
