#include "WorldGUIItemViewer.hpp"
#include "WorldGUIItemSelector.hpp"

using namespace CEGUI;

CWorldGUIItemViewer::CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIPullMenu("world_gui_item_viewer", pParentEntity, pParentWindow, PMD_TOP, -1, "DefaultWindow") {

  CWorldGUIItemSelector *pItemSelector = new CWorldGUIItemSelector(this, getDragWindow());
  pItemSelector->getRoot()->setSize(USize(UDim(0.667, -5), UDim(0.667, -5)));
  m_pWorldGUIItemSelector = pItemSelector;

  Window *pDoWindow = getDragWindow()->createChild("OgreTray/Group", "do");
  pDoWindow->setText("DO");
  pDoWindow->setPosition(UVector2(UDim(0, 0), UDim(0.667, 5)));
  pDoWindow->setSize(USize(UDim(0.667, -5), UDim(0.333, -5)));

  Window *pEquipmentWindow = getDragWindow()->createChild("OgreTray/Group", "equipment");
  pEquipmentWindow->setText("EQUIPMENT");
  pEquipmentWindow->setPosition(UVector2(UDim(0.667, 5), UDim(0.667, 5)));
  pEquipmentWindow->setSize(USize(UDim(0.333, -5), UDim(0.333, -5)));
}

void CWorldGUIItemViewer::onPullStarted() {
  pause(PAUSE_ATLAS_UPDATE);
  m_pWorldGUIItemSelector->stop();
}

void CWorldGUIItemViewer::onClosed() {
  unpause(PAUSE_ATLAS_UPDATE);
}

void CWorldGUIItemViewer::onOpened() {
  m_pWorldGUIItemSelector->start();
}
