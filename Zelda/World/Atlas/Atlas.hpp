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

#ifndef _ATLAS_HPP_
#define _ATLAS_HPP_

#include "../WorldEntity.hpp"
#include "../../Common/PauseManager/PauseListener.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"
#include "../../Common/Fader/Fader.hpp"
#include "../../Common/Message/MessageSwitchMap.hpp"

class CMap;
class CAerialCameraPerspective;
class CEntrance;

class CAtlas : public CWorldEntity,
               public CPauseListener,
               protected CPauseCaller,
               public CFaderCallback {
private:
  CMap *m_pCurrentMap;
  CMap *m_pNextMap;
  CWorldEntity *m_pPlayer;
  Ogre::Camera *m_pWorldCamera;
  CAerialCameraPerspective *m_pCameraPerspective;

  bool mFirstFrame;                                             //!< First frame after loading is not updated, since delta t can be enormous
  bool mFirstFrameUpdated;
  bool m_bSwitchingMaps;              //!< Is the map currently switch from current to next map
  ESwitchMapTypes m_eSwitchMapType;   //!< Type of the map switch
  std::string m_sNextMap;             //!< Next map after fading
  std::string m_sNextMapEntrance;     //!< Entrance in new map after fading
  bool m_bPlayerTargetReached;

  CFader mEllipticFader;
  CFader mAlphaFader;
public:
  CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode);
  ~CAtlas();

  CMap *getCurrentMap() const {return m_pCurrentMap;}

  void update(Ogre::Real tpf);
  void renderDebug(Ogre::Real tpf);
  void preRender(Ogre::Real tpf);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  bool frameStarted(const Ogre::FrameEvent& evt);
  bool frameEnded(const Ogre::FrameEvent& evt);

protected:
  void handleMessage(const CMessagePtr message);
  void updatePause(int iPauseType, bool bPause);

  virtual void fadeInCallback();
  virtual void fadeOutCallback();

private:
  CEntrance *getNextEntrancePtr() const;
};

#endif // _ATLAS_HPP_
