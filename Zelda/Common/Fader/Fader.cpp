/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

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
