#ifndef _GUI_HEARTS_DISPLAY_HPP_
#define _GUI_HEARTS_DISPLAY_HPP_

#include "../Common/GUI/GUIOverlay.hpp"
#include "../World/Hitpoints.hpp"


class CGUIHeartsDisplay : public CGUIOverlay {
private:
  CEGUI::Window *m_pRoot;

  static const Ogre::uint16 m_uiMaxHeartCount = 20;

  Hitpoints m_MaximalHitpoints;
  Hitpoints m_CurrentHitpoints;
public:
  CGUIHeartsDisplay(CEntity *pParentEntity, CEGUI::Window *pParentWindow);


  void changeCurrentHitpoints(Hitpoints hp);
  void changeMaximalHitpoints(Hitpoints hp);
private:

  void updateHitpoints();
};

#endif // _GUI_HEARTS_DISPLAY_HPP_
