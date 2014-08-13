#include "GUICounter.hpp"

using namespace CEGUI;

CGUICounter::CGUICounter(const std::string &id,
                         CEntity *pParentEntity,
                         CEGUI::Window *pParentWindow,
                         Ogre::Real fCountsPerSecond,
                         const CEGUI::String &sImage,
                         const CEGUI::UVector2 &vPosition)
  : CGUIOverlay(id, pParentEntity, pParentWindow),
    m_iCurrentCount(0),
    m_iDesiredCount(0),
    m_fTimeSinceLastAdded(0),
    m_fCountsPerSecond(fCountsPerSecond) {

  Window *pRoot = m_pParentWindow->createChild("DefaultWindow", id);
  pRoot->setSize(USize(UDim(0, 40), UDim(0, 60)));
  pRoot->setPosition(vPosition);


  Window *pPicture = pRoot->createChild("OgreTray/StaticImage", "image");
  pPicture->setPosition(UVector2(UDim(0, 5), UDim(0, 0)));
  pPicture->setSize(USize(UDim(0, 30), UDim(0, 30)));
  pPicture->setProperty("Image", sImage);
  pPicture->setProperty("FrameEnabled", "false");
  pPicture->setProperty("BackgroundEnabled", "false");

  m_pCounter = pRoot->createChild("OgreTray/ShadowedLabel", "counter");
  m_pCounter->setPosition(UVector2(UDim(0, 0), UDim(0.5, 0)));
  m_pCounter->setSize(USize(UDim(1, 0), UDim(0.5, 0)));
  updateText();
}

void CGUICounter::setCurrentCount(int iNewCount, bool bAnimate) {
  if (!bAnimate) {
    m_iCurrentCount = m_iDesiredCount = iNewCount;
  }
  else {
    m_iDesiredCount = iNewCount;
    m_fTimeSinceLastAdded = 0;
  }

  updateText();
}

void CGUICounter::update(Ogre::Real tpf) {
  if (m_iDesiredCount != m_iCurrentCount) {
    m_fTimeSinceLastAdded += tpf;

    if (m_fTimeSinceLastAdded * m_fCountsPerSecond >= 1) {
      m_iCurrentCount += m_fTimeSinceLastAdded * m_fCountsPerSecond;

      if (m_iCurrentCount >= m_iDesiredCount) {
        m_iCurrentCount = m_iDesiredCount;
      }
      m_fTimeSinceLastAdded = 0;

      updateText();
    }
  }
}

void CGUICounter::updateText() {
  m_pCounter->setText(PropertyHelper<int>::toString(m_iCurrentCount));
}
