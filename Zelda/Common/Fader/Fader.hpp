#ifndef _FADER_HPP_
#define _FADER_HPP_


#include <OgreOverlay.h>
#if OGRE_VERSION >= ((1 << 16) | (9 << 8) | 0)
#include <OgreOverlaySystem.h>
#endif

class CFaderCallback {
public:
  virtual void fadeInCallback() {}
  virtual void fadeOutCallback() {}
};

class CFader {
public:
  enum EFadeOperation {
    FADE_NONE,
    FADE_IN,
    FADE_OUT,
  };

private:
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

  void fade(Ogre::Real tpf) {
    if (m_eFadeOperation != FADE_NONE) {

      if (m_eFadeOperation == FADE_IN) {
	m_fCurrentDuration -= tpf;
	m_fProgress = m_fCurrentDuration / m_fTotalDuration;
	if (m_fProgress < 0.0) {
	  m_pOverlay->hide();
	  m_eFadeOperation = FADE_NONE;
	  if (m_pFaderCallback) {
	    m_pFaderCallback->fadeInCallback();
	  }
	}
      }
      else if (m_eFadeOperation == FADE_OUT) {
	m_fCurrentDuration += tpf;
	m_fProgress = m_fCurrentDuration / m_fTotalDuration;
	if (m_fProgress > 1.0) {
	  m_eFadeOperation = FADE_NONE;
	  if (m_pFaderCallback) {
	    m_pFaderCallback->fadeOutCallback();
	  }
	}
      }
    }
    m_PixelShaderParameters->setNamedConstant("progress", m_fProgress);
  }
};

#endif // _FADER_HPP_
