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

#include "GUIDebugPullMenu.hpp"
#include <OgreSkeletonManager.h>
#include "../Message/MessageDebug.hpp"
#include "../Message/MessageHandler.hpp"
#include "../Log.hpp"
#include "../Game.hpp"

using CEGUI::Window;
using CEGUI::UVector2;
using CEGUI::USize;
using CEGUI::UDim;
using CEGUI::PropertyHelper;
using CEGUI::WindowEventArgs;
using CEGUI::ToggleButton;
using CEGUI::Event;
using CEGUI::String;

CGUIDebugPullMenu::CGUIDebugPullMenu(CEntity *pParentEntity,
                                     CEGUI::Window *pParentWindow,
                                     EPullMenuPositions ePosition)
    : CGUIPullMenu("DebugPullMenu",
                     pParentEntity,
                     pParentWindow,
                     ePosition,
                     400) {
  getDragWindow()->setText("Debug");

  m_pContent = getDragWindow()->createChild("OgreTray/ScrollablePane", "pane");
  m_pContent->setSize(USize::one());

  float fPos = 0;
  createButton("OgreTray/Checkbox",
               "debug_drawer",
               "Toggle debug drawer",
               fPos)
      ->subscribeEvent(
          ToggleButton::EventSelectStateChanged,
          Event::Subscriber(&CGUIDebugPullMenu::onToggleDebugDrawer, this));
  createButton("OgreTray/Checkbox",
               "physics",
               "Toggle physics debug",
               fPos)
      ->subscribeEvent(
          ToggleButton::EventSelectStateChanged,
          Event::Subscriber(&CGUIDebugPullMenu::onTogglePhysics, this));

  m_pFrameStatsGroup = m_pContent->createChild("OgreTray/Group", "frame_stats");
  m_pFrameStatsGroup->setText("fps: 0");
  m_pFrameStatsGroup->setPosition(UVector2(UDim(0, 0), UDim(0, fPos)));
  m_pFrameStatsGroup->setSize(USize(UDim(1, 0), UDim(0, 150)));
  fPos += 155;

  createFrameStatsButton("av_fps", "Average FPS:", m_pAverageFps, 0);
  createFrameStatsButton("best_fps", "Best FPS:", m_pBestFps, 1);
  createFrameStatsButton("worst_fps", "Worst FPS:", m_pWorstFps, 2);
  createFrameStatsButton("batches", "Batches:", m_pBatches, 4);
  createFrameStatsButton("triangles", "Triangles:", m_pTriangles, 3);

createLabel("Entities:", m_pEntites, fPos);
createLabel("Ogre::Entities", m_pOgreEntites, fPos);
createLabel("Ogre::Skeletons", m_pOgreSkeletons, fPos);
}


CEGUI::Window *CGUIDebugPullMenu::createButton(const CEGUI::String &wnd,
                                               const CEGUI::String &id,
                                               const String &text,
                                               float &fPos) {
  Window *pButton = m_pContent->createChild(wnd, id);
  pButton->setSize(USize(UDim(1, 0), UDim(0, 30)));
  pButton->setPosition(UVector2(UDim(0, 0), UDim(0, fPos)));
  pButton->setText(text);
  fPos += 35;

  return pButton;
}

CEGUI::Window *CGUIDebugPullMenu::createLabel(const CEGUI::String &name,
                                              CEGUI::Window *&pWindow,
                                              float &fPos) {
  Window *pLabel = m_pContent->createChild("OgreTray/Label", name + "_label");
  pLabel->setSize(USize(UDim(0.5, 0), UDim(0, 30)));
  pLabel->setPosition(UVector2(UDim(0, 0), UDim(0, fPos)));
  pLabel->setText(name);
  pLabel->setProperty("HorzFormatting", "LeftAligned");

  pWindow = m_pContent->createChild("OgreTray/Label", name);
  pWindow->setSize(USize(UDim(0.5, 0), UDim(0, 30)));
  pWindow->setPosition(UVector2(UDim(0.5, 0), UDim(0, fPos)));
  pWindow->setProperty("HorzFormatting", "RightAligned");
  fPos += 35;

  return pLabel;
}

CEGUI::Window *CGUIDebugPullMenu::createFrameStatsButton(
    const CEGUI::String &id,
    const CEGUI::String &sLabel,
    CEGUI::Window *&pWindow,
    int iIndex) {
  Window *pLabel = m_pFrameStatsGroup->createChild("OgreTray/Label",
                                                   id + "_label");
  pLabel->setSize(USize(UDim(0.5, 0), UDim(1.f / 5.f, -2)));
  pLabel->setPosition(UVector2(UDim(0, 0), UDim(iIndex * 1.f / 5.f, 2)));
  pLabel->setText(sLabel);
  pLabel->setProperty("HorzFormatting", "LeftAligned");

  pWindow = m_pFrameStatsGroup->createChild("OgreTray/Label", id);
  pWindow->setSize(USize(UDim(0.5, 0), UDim(1.f / 5.f, -2)));
  pWindow->setPosition(UVector2(UDim(0.5, 0), UDim(iIndex * 1.f / 5.f, 2)));
  pWindow->setProperty("HorzFormatting", "RightAligned");

  return pWindow;
}

bool CGUIDebugPullMenu::onToggleDebugDrawer(const CEGUI::EventArgs &args) {
  ToggleButton *pTB = dynamic_cast<ToggleButton*>(
      dynamic_cast<const WindowEventArgs&>(args).window);
  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageDebug>(__MSG_LOCATION__,
                                      CMessageDebug::DM_TOGGLE_DEBUG_DRAWER,
                                      pTB->isSelected()));
  return true;
}

bool CGUIDebugPullMenu::onTogglePhysics(const CEGUI::EventArgs &args) {
  ToggleButton *pTB = dynamic_cast<ToggleButton*>(
      dynamic_cast<const WindowEventArgs&>(args).window);
  CMessageHandler::getSingleton().addMessage(
      std::make_shared<CMessageDebug>(__MSG_LOCATION__,
                                      CMessageDebug::DM_TOGGLE_PHYSICS,
                                      pTB->isSelected()));
  return true;
}

void CGUIDebugPullMenu::update(Ogre::Real tpf) {
  CGUIPullMenu::update(tpf);
  if (getDragState() != DS_SLEEPING) {
    const Ogre::RenderTarget::FrameStats stats(
        CGame::getSingleton().getRenderWindow()->getStatistics());
    Ogre::SceneManager *sceneMgr = CGame::getSingleton().getSceneManager();

    m_pFrameStatsGroup->setText("FPS: " + PropertyHelper<int>::toString(
        stats.lastFPS));
    m_pAverageFps->setText(PropertyHelper<int>::toString(stats.avgFPS));
    m_pBestFps->setText(PropertyHelper<int>::toString(stats.bestFPS));
    m_pWorstFps->setText(PropertyHelper<int>::toString(stats.worstFPS));
    m_pBatches->setText(PropertyHelper<int>::toString(stats.batchCount));
    m_pTriangles->setText(PropertyHelper<int>::toString(stats.triangleCount));

    m_pEntites->setText(PropertyHelper<int>::toString(
        CEntity::getNumberOfInstances()));

    uint32_t count = 0;
    for (auto i =
             sceneMgr->getMovableObjectIterator("Entity");
         i.hasMoreElements();
         ) {
      i.getNext();
      count++;
    }
    m_pOgreEntites->setText(PropertyHelper<int>::toString(count));

    count = 0;
    for (auto i =
             Ogre::SkeletonManager::getSingleton().getResourceIterator();
         i.hasMoreElements();
         ) {
      i.getNext();
      count++;
    }
    m_pOgreSkeletons->setText(PropertyHelper<int>::toString(count));
  }
}
