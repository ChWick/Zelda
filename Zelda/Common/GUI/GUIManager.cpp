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

#include "GUIManager.hpp"
#include <OgreSceneManager.h>
#include "../Game.hpp"
#include <dependencies/OgreSdkUtil/SdkTrays.h>

using namespace CEGUI;

template<> CGUIManager* Ogre::Singleton<CGUIManager>::msSingleton = 0;

CGUIManager* CGUIManager::getSingletonPtr(void)
{
  return msSingleton;
}
CGUIManager& CGUIManager::getSingleton(void)
{
  assert( msSingleton );  return ( *msSingleton );
}

CGUIManager::CGUIManager(Ogre::SceneManager *pSceneManager, Ogre::RenderTarget &target)
: m_pSceneManager(pSceneManager),
  m_nRenderQueue(Ogre::RENDER_QUEUE_OVERLAY),
  m_bPostQueue(false),
  m_bRenderPause(false),
  m_vNativeRes(target.getWidth(), target.getHeight()),
  m_MouseMoving(MD_COUNT, false),
  m_fMouseSpeedX(0),
  m_fMouseSpeedY(0),
  m_fMouseAcceleration(2),
  m_fMouseInitialSpeed(50) {
  CInputListenerManager::getSingleton().addInputListener(this);

  OgreBites::SdkTrayManager *pTrayMgr = CGame::getSingleton().showLoadingBar(1, 1);
  pTrayMgr->loadBarSetCaption("GUI ...");
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");

  Ogre::LogManager::getSingletonPtr()->logMessage("    creating bootstrapSystem");
  pTrayMgr->userUpdateLoadBar("Creating bootstrap system", 0.1);
  m_pCEGuiOgreRenderer = &CEGUI::OgreRenderer::bootstrapSystem(target);
  m_pCEGuiOgreRenderer->setFrameControlExecutionEnabled(false);

  Ogre::LogManager::getSingletonPtr()->logMessage("    setting up resource paths");
  pTrayMgr->userUpdateLoadBar("Setting up resource paths", 0.1);
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  Ogre::LogManager::getSingletonPtr()->logMessage("    creating scheme");
  pTrayMgr->userUpdateLoadBar("Creating scheme", 0.1);
  createResources();

  pSceneManager->addRenderQueueListener(this);

  Ogre::LogManager::getSingletonPtr()->logMessage("    creating root window");
  pTrayMgr->userUpdateLoadBar("Initializing the root window", 0.1);
  CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "MasterRoot");
  m_pRoot = guiRoot;
  //guiRoot->setAlpha(0);
  guiRoot->setSize(USize(UDim(1, 0), UDim(1, 0)));
  //guiRoot->setProperty("BackgroundEnabled", "false");
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");
  //CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseInvisible");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2f(0,0));
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setConstraintArea(NULL);


  pTrayMgr->userUpdateLoadBar("Creating fonts", 0.2);
  createFreeTypeFont("dejavusans12", 12, "DejaVuSans.ttf");
  createFreeTypeFont("dejavusans8", 8, "DejaVuSans.ttf");
  createFreeTypeFont("dejavusans20", 20, "DejaVuSans.ttf");
  CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "dejavusans12" );
  guiRoot->setFont("dejavusans12");


  pTrayMgr->userUpdateLoadBar("Creating gui components", 0.2);

  pTrayMgr->userUpdateLoadBar("done...", 0.2);

  Sizef vSize = CGUIManager::getSingleton().getNativeRes();

  Ogre::LogManager::getSingleton().logMessage("GUIManager initialized...");
  CGame::getSingleton().hideLoadingBar();
}
CGUIManager::~CGUIManager() {
  CInputListenerManager::getSingleton().removeInputListener(this);
  m_pSceneManager->removeRenderQueueListener(this);

  /*while (m_lGUIOverlays.size() > 0) {
    delete m_lGUIOverlays.front();
    m_lGUIOverlays.pop_front();
  }*/
  m_lGUIOverlays.clear();

  if (CEGUI::System::getSingletonPtr()) {CEGUI::OgreRenderer::destroySystem();}
}
void CGUIManager::update(Ogre::Real tpf) {
  CEGUI::System::getSingleton().injectTimePulse(tpf);

  // move mouse cursor
  if (CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible()) {
    m_fMouseSpeedX += m_fMouseSpeedX * m_fMouseAcceleration * tpf;
    m_fMouseSpeedY += m_fMouseSpeedY * m_fMouseAcceleration * tpf;
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(m_fMouseSpeedX * tpf, m_fMouseSpeedY * tpf);
  }

  if (m_bRenderPause) {return;}

  // updating of gui overlays is done via the CEntity logic
  /*for (CGUIOverlay *pOverlay : m_lGUIOverlays) {
    pOverlay->update(tpf);
  }*/
}
void CGUIManager::renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue) {
   // make sure you check the invocation string, or you can end up rendering the GUI multiple times
   // per frame due to shadow cameras.
   if ( !m_bPostQueue && m_nRenderQueue == id && invocation == "" && !m_bRenderPause)
   {
     CEGUI::System::getSingleton().renderAllGUIContexts();
   }
}

void CGUIManager::renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue) {
   if ( m_bPostQueue && m_nRenderQueue == id && invocation == "" && !m_bRenderPause)
   {
     CEGUI::System::getSingleton().renderAllGUIContexts();
   }
}

//-------------------------------------------------------------------------------------
bool CGUIManager::keyPressed( const OIS::KeyEvent &arg ) {
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(arg.key));
  sys.getDefaultGUIContext().injectChar(arg.text);

  switch (arg.key) {
  case OIS::KC_LEFT:
    m_MouseMoving[MD_LEFT] = true;
    m_fMouseSpeedX = -m_fMouseInitialSpeed;
    break;
  case OIS::KC_RIGHT:
    m_MouseMoving[MD_RIGHT] = true;
    m_fMouseSpeedX = m_fMouseInitialSpeed;
    break;
  case OIS::KC_UP:
    m_MouseMoving[MD_UP] = true;
    m_fMouseSpeedY = -m_fMouseInitialSpeed;
    break;
  case OIS::KC_DOWN:
    m_MouseMoving[MD_DOWN] = true;
    m_fMouseSpeedY = m_fMouseInitialSpeed;
    break;
  case OIS::KC_RETURN:
    if (CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible()) {
      sys.getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
    }
    break;
  default:
    break;
  }

  return true;
}

bool CGUIManager::keyReleased( const OIS::KeyEvent &arg ) {
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(arg.key));

  switch (arg.key) {
  case OIS::KC_LEFT:
    m_MouseMoving[MD_LEFT] = false;
    m_fMouseSpeedX = 0;
    break;
  case OIS::KC_RIGHT:
    m_MouseMoving[MD_RIGHT] = false;
    m_fMouseSpeedX = 0;
    break;
  case OIS::KC_UP:
    m_MouseMoving[MD_UP] = false;
    m_fMouseSpeedY = 0;
    break;
  case OIS::KC_DOWN:
    m_MouseMoving[MD_DOWN] = false;
    m_fMouseSpeedY = 0;
    break;
  case OIS::KC_RETURN:
    if (CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().isVisible()) {
      sys.getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
    }
    break;
  default:
    break;
  void addImagesetResource(const SImagesetResource &res);
  }

  return true;
}
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
  switch (buttonID) {
  case OIS::MB_Left:
    return CEGUI::LeftButton;

  case OIS::MB_Right:
    return CEGUI::RightButton;

  case OIS::MB_Middle:
    return CEGUI::MiddleButton;

  default:
    return CEGUI::LeftButton;
  }
}
bool CGUIManager::mouseMoved( const OIS::MouseEvent &arg ) {
  CEGUI::System &sys = CEGUI::System::getSingleton();
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  // Scroll wheel.
  if (arg.state.Z.rel)
    sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);


  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseMove, args);

  return true;
}
bool CGUIManager::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseButtonDown, args);

  return true;
}
bool CGUIManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseButtonUp, args);

  return true;
}
bool CGUIManager::touchMoved(const OIS::MultiTouchEvent& arg) {
  //sys.getDefaultGUIContext().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseMove, args);

  return true;
}
bool CGUIManager::touchPressed(const OIS::MultiTouchEvent& arg) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseButtonDown, args);

  return true;
}
bool CGUIManager::touchReleased(const OIS::MultiTouchEvent& arg) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
  CEGUI::MouseEventArgs args(NULL);
  args.position = CEGUI::Vector2f(arg.state.X.abs, arg.state.Y.abs);
  args.moveDelta = CEGUI::Vector2f(arg.state.X.rel, arg.state.Y.rel);
  CEGUI::GlobalEventSet::getSingleton().fireEvent(CEGUI::Window::EventMouseButtonUp, args);

  return true;
}
bool CGUIManager::touchCancelled(const OIS::MultiTouchEvent& evt) {
  return true;
}

void CGUIManager::createFreeTypeFont(const CEGUI::String &name, int size, const CEGUI::String &ttfFile) {
  CEGUI::FontManager::getSingleton().createFreeTypeFont(name, size, true, ttfFile, "Fonts", CEGUI::ASM_Both, m_vNativeRes);
  m_vFonts.push_back(name);
}

void CGUIManager::addImagesetResource(const SImagesetResource &res) {
  CEGUI::ImageManager::getSingleton().loadImageset(res.m_sImagesetName + ".imageset");
  m_lActiveImagesetResources.push_back(res);
}

void CGUIManager::createResources() {
  m_bRenderPause = false;

  // Scheme
  CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");

  // Imagesets
  addImagesetResource(SImagesetResource("OgreTray", "OgreTrayImages.png"));

#ifdef INPUT_MOUSE
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");
#else
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseInvisible");
#endif
}
void CGUIManager::destroyResources() {
  m_bRenderPause = true;

  for (SImagesetResource res : m_lActiveImagesetResources) {
    CEGUI::ImageManager::getSingleton().destroyImageCollection(res.m_sImagesetName);
  }

  m_lActiveImagesetResources.clear();
}
void CGUIManager::reloadResources() {
  for (SImagesetResource res : m_lActiveImagesetResources) {
    m_pCEGuiOgreRenderer->getTexture(res.m_sImagesetName).loadFromFile(res.m_sTextureName, "Imagesets");
  }

  for (auto &sFontName : m_vFonts) {
    CEGUI::FontManager::getSingleton().get(sFontName).notifyDisplaySizeChanged(m_vNativeRes);
  }
}
void CGUIManager::resize(const CEGUI::Sizef &vSize) {
  m_vNativeRes = vSize;
  m_pCEGuiOgreRenderer->setDisplaySize(vSize);

  CEGUI::String smallfont("dejavusans8");
  CEGUI::String bigfont("dejavusans12");
  if (vSize.d_height > 800) {
    smallfont = "dejavusans20";
  }
  else if (vSize.d_height > 500) {
    smallfont = "dejavusans12";
  }

  if (vSize.d_height > 600) {
    bigfont = "dejavusans20";
  }
}
void CGUIManager::changeTouchButtonSize(float fSize) {
  for (CGUIOverlay *pOverlay : m_lGUIOverlays) {
    pOverlay->changeTouchButtonSize(fSize);
  }
}

void CGUIManager::onGUIScalingChanged(float fScaling) {
  for (CGUIOverlay *pOverlay : m_lGUIOverlays) {
    pOverlay->onGUIScalingChanged(fScaling);
  }
}
