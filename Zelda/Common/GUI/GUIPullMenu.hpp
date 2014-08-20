#ifndef _GUI_PULL_MENU_HPP_
#define _GUI_PULL_MENU_HPP_

#include "GUIOverlay.hpp"
#include "../Input/InputListener.hpp"

class CGUIPullMenu :
  public CGUIOverlay,
  public CInputListener {

public:
  enum EPullMenuPositions {
    PMD_LEFT = 1,
    PMD_TOP = 2,
    PMD_RIGHT = 4,
    PMD_BOTTOM = 8,

    PMD_HORIZONTAL = PMD_LEFT | PMD_RIGHT,
    PMD_VERTICAL = PMD_TOP | PMD_BOTTOM,
  };
private:
  enum EDragState {
    DS_SLEEPING,
    DS_DRAGGING,
    DS_CLOSING,
    DS_OPENING,
    DS_OPEN,
  };

private:
  const EPullMenuPositions m_ePosition;
  const float m_fSize;
  const float m_fDragButtonSize;

  CEGUI::Window *m_pDragButton;
  CEGUI::Window *m_pDragWindow;
  bool m_bPressed;

  EDragState m_eDragState;
  float m_fDragVelocity;
  float m_fLastDragPos;
  float m_fTimeSinceLastTouchMoveEvent;
public:
  CGUIPullMenu(const std::string &id,
               CEntity *pParentEntity,
               CEGUI::Window *pParentWindow,
               EPullMenuPositions ePosition,
               const float &fPixSize
               );

  virtual ~CGUIPullMenu();

  void update(Ogre::Real tpf);

  CEGUI::Window *getDragWindow() const {return m_pDragWindow;}
protected:
  virtual void onPullStarted() {}
  virtual void onOpened() {}
  virtual void onPullEnded() {}
  virtual void onClosed() {}

private:
  void pressReleased();
  void updateDragButtonPosition(const CEGUI::EventArgs&);
  void updateDragBar(float fPosY);

  bool onDragPressed(const CEGUI::EventArgs&);
  bool onDragReleased(const CEGUI::EventArgs&);
  bool onDragMoved(const CEGUI::EventArgs&);
  bool onDragEnter(const CEGUI::EventArgs&);
  bool onDragLeft(const CEGUI::EventArgs&);

  bool touchReleased(const OIS::MultiTouchEvent& evt) {pressReleased(); return true;}
  bool touchCancelled(const OIS::MultiTouchEvent& evt) {pressReleased();return true;}
  bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {m_bPressed = true; return true;}
  bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {pressReleased(); return true;}
};

#endif // _GUI_PULL_MENU_HPP_
