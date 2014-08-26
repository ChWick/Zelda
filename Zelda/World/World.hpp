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

#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "../Common/GameLogic/GameState.hpp"
#include <OgrePrerequisites.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined(__LP64__)
#  include <Carbon/Carbon.h>
#endif
#include <OgreMaterial.h>

class CPlayer;
class CAtlas;
class CAerialCameraPerspective;
class CWorldGUI;

class CWorld : public CGameState {
private:
  CPlayer *m_pPlayer;
  CAtlas *m_pAtlas;
  Ogre::Camera *m_pWorldCamera;
  CAerialCameraPerspective *m_pCameraPerspective;
  CWorldGUI *m_pWorldGUI;

  Ogre::MaterialPtr m_pWaterSideWaveMaterial;
  Ogre::Real m_fWaderSideWaveMaterialNextImageIn;
  int m_iWaterSideWaveMaterialCurrentImage;
public:
  CWorld();
  ~CWorld();

  void update(Ogre::Real tpf);
  void preRender(Ogre::Real tpf);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
};

#endif // _WORLD_HPP_
