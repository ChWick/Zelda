#include "Fader.hpp"
#include <OgreTechnique.h>
#include <OgrePass.h>

const Ogre::String CFader::ELLIPTIC_FADER("EllipticFade");
const Ogre::String CFader::ALPHA_FADER("AlphaFade");

CFader::CFader(const Ogre::String &overlay, CFaderCallback *pCallback)
    :
  mType(overlay),
  m_fProgress(0),
  m_fCurrentDuration(0),
  m_fTotalDuration(0),
  m_pFaderCallback(pCallback),
  m_eFadeOperation(FADE_NONE) {
  m_pOverlay = Ogre::OverlayManager::getSingleton().getByName(overlay);
  m_pOverlay->hide();
  m_PixelShaderParameters = m_pOverlay->get2DElementsIterator().getNext()->getMaterial()->getSupportedTechnique(0)->getPass(0)->getFragmentProgramParameters();
}

void CFader::setFadeCenter(const Ogre::Vector2 &vCenter) {
  if (mType == ELLIPTIC_FADER) {
    m_PixelShaderParameters->setNamedConstant("offset", vCenter);
  }
}

void CFader::fade(Ogre::Real tpf) {
    if (m_eFadeOperation != FADE_NONE) {

      if (m_eFadeOperation == FADE_IN) {
        if (m_fProgress < 0.0) {
          m_pOverlay->hide();
          m_eFadeOperation = FADE_NONE;
          if (m_pFaderCallback) {
            m_pFaderCallback->fadeInCallback();
          }
        }
        m_fCurrentDuration -= tpf;
        m_fProgress = m_fCurrentDuration / m_fTotalDuration;
      }
      else if (m_eFadeOperation == FADE_OUT) {
        if (m_fProgress > 1.0) {
          m_eFadeOperation = FADE_NONE;
          if (m_pFaderCallback) {
            m_pFaderCallback->fadeOutCallback();
          }
        }
        m_fCurrentDuration += tpf;
        m_fProgress = m_fCurrentDuration / m_fTotalDuration;
      }
    }
    m_PixelShaderParameters->setNamedConstant("progress", std::min<Ogre::Real>(1, std::max<Ogre::Real>(0, m_fProgress)));
  }
