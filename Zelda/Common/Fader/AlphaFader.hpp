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

#ifndef ALPHA_FADER_HPP
#define ALPHA_FADER_HPP

#include "Fader.hpp"

class CAlphaFader : public CFader {
private:
  Ogre::Real m_fAlpha;
  Ogre::Real m_fCurrentDuration;
  Ogre::Real m_fTotalDuration;
  CFaderCallback *m_pFaderCallback;
  Ogre::TextureUnitState *m_pTextUnitState;
  Ogre::Overlay *m_pOverlay;
  EFadeOperation m_eFadeOperation;

  Ogre::GpuProgramParametersSharedPtr m_SpriteVertexShaderParameters;
  Ogre::GpuProgramParametersSharedPtr m_SpritePixelShaderParameters;
public:
  CAlphaFader(CFaderCallback *pCallback = NULL)
    :
    m_fAlpha(0),
    m_fCurrentDuration(0),
    m_fTotalDuration(0),
    m_pFaderCallback(pCallback),
    m_eFadeOperation(FADE_NONE) {
    Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().getByName("FadeMaterial");
#ifdef USE_SPRITE_SHADER
    CShaderManager::getSingleton().addAlphaBlendTextureShader(matptr);
    Ogre::Pass *pPass = matptr->getTechnique(0)->getPass(0);
    m_SpritePixelShaderParameters = pPass->getFragmentProgram()->createParameters();
    m_SpriteVertexShaderParameters = pPass->getVertexProgram()->createParameters();
    pPass->setFragmentProgramParameters(m_SpritePixelShaderParameters);
    pPass->setVertexProgramParameters(m_SpriteVertexShaderParameters);
#endif
    m_pTextUnitState = matptr->getTechnique(0)->getPass(0)->getTextureUnitState(0);
    m_pOverlay = Ogre::OverlayManager::getSingleton().getByName("FadeOverlay");
    m_pOverlay->hide();
  }
};


#endif
