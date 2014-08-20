#include "GUIDebugPullMenu.hpp"
#include "../Message/MessageDebug.hpp"
#include "../Message/MessageHandler.hpp"

using namespace CEGUI;

CGUIDebugPullMenu::CGUIDebugPullMenu(CEntity *pParentEntity,
                                     CEGUI::Window *pParentWindow,
                                     EPullMenuPositions ePosition)
  : CGUIPullMenu("DebugPullMenu", pParentEntity, pParentWindow, ePosition, 400) {
  
  getDragWindow()->setText("Debug");

  m_pContent = getDragWindow()->createChild("OgreTray/ScrollablePane", "pane");
  m_pContent->setSize(USize::one());

  float fPos = 0;
  createButton("OgreTray/Checkbox", "physics", "Toggle physics debug", fPos)->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CGUIDebugPullMenu::onTogglePhysics, this));
}


CEGUI::Window *CGUIDebugPullMenu::createButton(const CEGUI::String &wnd, const CEGUI::String &id, const String &text, float &fPos) {
  Window *pButton = m_pContent->createChild(wnd, id);
  pButton->setSize(USize(UDim(1, 0), UDim(0, 30)));
  pButton->setPosition(UVector2(UDim(0, 0), UDim(0, fPos)));
  pButton->setText(text);
  fPos += 35;

  return pButton;
}


bool CGUIDebugPullMenu::onTogglePhysics(const CEGUI::EventArgs &args) {
  CMessageHandler::getSingleton().addMessage(new CMessageDebug(CMessageDebug::DM_TOGGLE_PHYSICS, true));
  return true;
}