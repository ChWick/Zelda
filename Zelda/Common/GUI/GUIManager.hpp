/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <CEGUI/CEGUI.h>

#include <OgreSingleton.h>
#include <OgrePrerequisites.h>
#include <OgreRenderQueueListener.h>
#include "../Input/InputListener.hpp"
#include <vector>
#include "GUIOverlay.hpp"
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class CGUIManager :
  public CInputListener,
  public Ogre::Singleton<CGUIManager>,
  public Ogre::RenderQueueListener {
public:
  struct SImagesetResource {
    SImagesetResource(const CEGUI::String &sImagesetName, const CEGUI::String &sTextureName)
      : m_sImagesetName(sImagesetName),
        m_sTextureName(sTextureName) {
    }
    SImagesetResource(const SImagesetResource &src) = default;

    CEGUI::String m_sImagesetName;
    CEGUI::String m_sTextureName;
  };
private:
  enum EMouseDirections {
    MD_LEFT = 0,
    MD_RIGHT,
    MD_UP,
    MD_DOWN,
    MD_COUNT
  };

  Ogre::SceneManager *m_pSceneManager;

  CEGUI::OgreRenderer* m_pCEGuiOgreRenderer;
  //! ImageCodec to use.  Set in subclass constructor, may be 0.
  CEGUI::ImageCodec* m_pCEGuiImageCodec;
  //! ResourceProvider to use.  Set in subclass constructor, may be 0.
  CEGUI::ResourceProvider* m_pCEGuiResourceProvider;

  CEGUI::Window *m_pRoot;

  bool m_bRenderPause;

  CEGUI::Sizef m_vNativeRes;
  std::vector<CEGUI::String> m_vFonts;
  std::list<CGUIOverlay *> m_lGUIOverlays;
  std::list<SImagesetResource> m_lActiveImagesetResources;

  std::vector<bool> m_MouseMoving;
  float m_fMouseSpeedX;
  float m_fMouseSpeedY;
  const float m_fMouseAcceleration;
  const float m_fMouseInitialSpeed;
public:
  static CGUIManager& getSingleton(void);
  static CGUIManager* getSingletonPtr(void);

  CGUIManager(Ogre::SceneManager *pSceneManager, Ogre::RenderTarget &target);
  ~CGUIManager();

  const CEGUI::Sizef &getNativeRes() const {return m_vNativeRes;}


  void addImagesetResource(const SImagesetResource &res);
  void createResources();
  void destroyResources();
  void reloadResources();

  void update(Ogre::Real tpf);

  void addGUIOverlay(CGUIOverlay *pOverlay) {m_lGUIOverlays.push_back(pOverlay);}
  //void destroyGUIOverlay(CGUIOverlay *pOverlay) {m_lGUIOverlays.remove(pOverlay); delete pOverlay;}
  void removeGUIOverlay(CGUIOverlay *pOverlay) {m_lGUIOverlays.remove(pOverlay);}

  CEGUI::OgreRenderer *getRenderer() const {return m_pCEGuiOgreRenderer;}
  CEGUI::Window *getRoot() const {return m_pRoot;}

  // Ogre::RenderQueueListener
  void renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue);
  void renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );

  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

  // OIS::MultitouchListener
  virtual bool touchMoved(const OIS::MultiTouchEvent& evt);
  virtual bool touchPressed(const OIS::MultiTouchEvent& evt);
  virtual bool touchReleased(const OIS::MultiTouchEvent& evt);
  virtual bool touchCancelled(const OIS::MultiTouchEvent& evt);

  void resize(const CEGUI::Sizef &vSize);
  void changeTouchButtonSize(float fSize);
  void onGUIScalingChanged(float fScaling);
private:
  void createFreeTypeFont(const CEGUI::String &name, int size, const CEGUI::String &ttfFile);
private:
   Ogre::RenderQueueGroupID m_nRenderQueue; //!< When we draw the UI.
   bool m_bPostQueue;                       //!< Whether to draw CEGUI immediately before or after m_nRenderQueue
};

#endif
