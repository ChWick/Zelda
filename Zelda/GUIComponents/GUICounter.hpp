#ifndef _GUI_COUNTER_HPP_
#define _GUI_COUNTER_HPP_

#include "../Common/GUI/GUIOverlay.hpp"

class CGUICounter : public CGUIOverlay {
private:
  int m_iCurrentCount;
  int m_iDesiredCount;
  Ogre::Real m_fTimeSinceLastAdded;
  const Ogre::Real m_fCountsPerSecond;

  CEGUI::Window *m_pCounter;
public:
  CGUICounter(const std::string &id,
              CEntity *pParentEntity,
              CEGUI::Window *pParentWindow,
              Ogre::Real fCountsPerSecond,
              const CEGUI::String &sImage,
              const CEGUI::UVector2 &vPosition = CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));

  void setCurrentCount(int iNewCount, bool bAnimate = true);

  void update(Ogre::Real tpf);

private:
  void updateText();
};

#endif // _GUI_COUNTER_HPP_
