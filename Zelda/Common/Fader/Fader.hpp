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

#ifndef _FADER_HPP_
#define _FADER_HPP_

#include <OgrePrerequisites.h>

#if OGRE_VERSION >= ((1 << 16) | (9 << 8) | 0)
#include <OgreOverlaySystem.h>
#endif

#include <OgreOverlay.h>

class CFaderCallback {
public:
  virtual void fadeInCallback() {}
  virtual void fadeOutCallback() {}
};

class CFader {
public:
  const static Ogre::String ELLIPTIC_FADER;
  const static Ogre::String ALPHA_FADER;

  enum EFadeOperation {
    FADE_NONE,
    FADE_IN,
    FADE_OUT,
  };

private:
  const Ogre::String mType;
  Ogre::Real m_fProgress;                                                   //!< Progress of fading [0,1]
  Ogre::Real m_fCurrentDuration;
  Ogre::Real m_fTotalDuration;
  CFaderCallback *m_pFaderCallback;
  Ogre::Overlay *m_pOverlay;
  EFadeOperation m_eFadeOperation;

  Ogre::GpuProgramParametersSharedPtr m_PixelShaderParameters;
public:
  CFader(const Ogre::String &overlay, CFaderCallback *pCallback = NULL);

  bool isFading() const {return m_eFadeOperation != FADE_NONE;}
  void setVisible(bool bShow) {if (bShow){m_pOverlay->show();} else {m_pOverlay->hide();}}

  void setFadeCenter(const Ogre::Vector2 &vCenter);

  void startFadeIn(Ogre::Real fDuration) {
    if (fDuration < 0) fDuration = -fDuration;

    m_fProgress = 1.0;
    m_fTotalDuration = fDuration;
    m_fCurrentDuration = fDuration;
    m_eFadeOperation = FADE_IN;
    m_pOverlay->show();
  }
  void startFadeOut(Ogre::Real fDuration) {
    if (fDuration < 0) fDuration = -fDuration;

    m_fProgress = 0;
    m_fTotalDuration = fDuration;
    m_fCurrentDuration = 0;
    m_eFadeOperation = FADE_OUT;
    m_pOverlay->show();
  }

  void fade(Ogre::Real tpf);
};

#endif // _FADER_HPP_
