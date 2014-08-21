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

#ifndef _INPUT_LISTENER_H_
#define _INPUT_LISTENER_H_

#include <OIS.h>
#include <OgreSingleton.h>
#include <OgreRenderWindow.h>

class CInputListener {
private:
  bool m_bInputListenerEnabled;
public:
  CInputListener() {m_bInputListenerEnabled = true;}
  virtual ~CInputListener();
  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg ) {return true;}
  virtual bool keyReleased( const OIS::KeyEvent &arg ) {return true;}
  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg ) {return true;}
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
  // OIS::MultiTouchListener
  virtual bool touchMoved(const OIS::MultiTouchEvent& evt) {return true;}
  virtual bool touchPressed(const OIS::MultiTouchEvent& evt) {return true;}
  virtual bool touchReleased(const OIS::MultiTouchEvent& evt) {return true;}
  virtual bool touchCancelled(const OIS::MultiTouchEvent& evt) {return true;}

  bool isInputListenerEnabled() const {return m_bInputListenerEnabled;}
  void setInputListenerEnabled(bool bEnable) {m_bInputListenerEnabled = bEnable;}
};

class CInputListenerManager :
  public Ogre::Singleton<CInputListenerManager>,
#if (OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS) && (OGRE_PLATFORM != OGRE_PLATFORM_ANDROID)
  public OIS::KeyListener,
  public OIS::MouseListener
#else
  public OIS::MultiTouchListener
#endif 
{
private:
  Ogre::list<CInputListener*>::type m_ListenerList;
  Ogre::RenderWindow *&mWindow;
public:
  CInputListenerManager(Ogre::RenderWindow *&window)
    : mWindow(window) {
  }

  void addInputListener(CInputListener *pListener) {
    m_ListenerList.push_back(pListener);
  }
  void removeInputListener(CInputListener *pListener) {
    m_ListenerList.remove(pListener);
  }
  bool hasInputListener(CInputListener *pListener) {
    for (auto l : m_ListenerList) {
      if (l == pListener) {
	return true;
      }
    }
    return false;
  }

  static CInputListenerManager& getSingleton(void);
  static CInputListenerManager* getSingletonPtr(void);
  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg ) {
    for (auto  pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->keyPressed(arg)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool keyReleased( const OIS::KeyEvent &arg ) {
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->keyReleased(arg)) {
	  break;
	}
      }
    }
    return true;
  }
  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg ) {
    OIS::MouseState state = arg.state;
    transformInputState(state);
    OIS::MouseEvent orientedEvt((OIS::Object*)arg.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->mouseMoved(orientedEvt)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    OIS::MouseState state = arg.state;
    transformInputState(state);
    OIS::MouseEvent orientedEvt((OIS::Object*)arg.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->mousePressed(orientedEvt, id)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    OIS::MouseState state = arg.state;
    transformInputState(state);
    OIS::MouseEvent orientedEvt((OIS::Object*)arg.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->mouseReleased(orientedEvt, id)) {
	  break;
	}
      }
    }
    return true;
  }
  // OIS::MultitouchListener
  virtual bool touchMoved(const OIS::MultiTouchEvent& evt) {
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->touchMoved(orientedEvt)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool touchPressed(const OIS::MultiTouchEvent& evt) {
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->touchPressed(orientedEvt)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool touchReleased(const OIS::MultiTouchEvent& evt) {
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->touchReleased(orientedEvt)) {
	  break;
	}
      }
    }
    return true;
  }
  virtual bool touchCancelled(const OIS::MultiTouchEvent& evt) {
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    for (auto pListener : m_ListenerList) {
      if (pListener->isInputListenerEnabled()) {
	if (!pListener->touchCancelled(orientedEvt)) {
	  break;
	}
      }
    }
    return true;
  }
private:

  template <class T>
  void transformInputState (T &state) {
#if (OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0)
    int w = mWindow->getViewport(0)->getActualWidth();
    int h = mWindow->getViewport(0)->getActualHeight();
    int absX = state.X.abs;
    int absY = state.Y.abs;
    int relX = state.X.rel;
    int relY = state.Y.rel;

    switch (mWindow->getViewport(0)->getOrientationMode()) {
  case Ogre::OR_DEGREE_0:
    break;
  case Ogre::OR_DEGREE_90:
    state.X.abs = w - absY;
    state.Y.abs = absX;
    state.X.rel = -relY;
    state.Y.rel = relX;
    break;
  case Ogre::OR_DEGREE_180:
    state.X.abs = w - absX;
    state.Y.abs = h - absY;
    state.X.rel = -relX;
    state.Y.rel = -relY;
    break;
  case Ogre::OR_DEGREE_270:
    state.X.abs = absY;
    state.Y.abs = h - absX;
    state.X.rel = relY;
    state.Y.rel = -relX;
    break;
  }
#endif
  }
};


#endif //_INPUT_LISTENER_H_
