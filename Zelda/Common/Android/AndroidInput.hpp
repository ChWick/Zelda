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

#ifndef _ANDROID_INPUT_HPP_
#define _ANDROID_INPUT_HPP_

#include "OIS.h"
#include "../InputListener.hpp"

/*=============================================================================
  | Android input handling
  =============================================================================*/
class AndroidMultiTouch : public OIS::MultiTouch
{
public:
  AndroidMultiTouch():OIS::MultiTouch("Ogre", false, 0, 0){}
        
  /** @copydoc Object::setBuffered */
  virtual void setBuffered(bool buffered){}
        
  /** @copydoc Object::capture */
  virtual void capture(){}
        
  /** @copydoc Object::queryInterface */
  virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
        
  /** @copydoc Object::_initialize */
  virtual void _initialize(){}

  void setStatesToNone(int n) {
    for (int i = n; i < mStates.size(); i++) {
      mStates[i].touchType = OIS::MT_None;
    }
  }
        
  OIS::MultiTouchState &getMultiTouchState(int i){
    while(i >= mStates.size()){
      Ogre::RenderWindow* pRenderWnd = static_cast<Ogre::RenderWindow*>(Ogre::Root::getSingleton().getRenderTarget("OgreWindow"));
      if(pRenderWnd)
	{
	  OIS::MultiTouchState state;
	  state.width = pRenderWnd->getWidth();
	  state.height = pRenderWnd->getHeight();
	  mStates.push_back(state);
	}
    }
    return mStates[i];
  }
};
    
class AndroidKeyboard : public OIS::Keyboard
{
public:
  AndroidKeyboard():OIS::Keyboard("Ogre", false, 1, 0){}
        
  /** @copydoc Object::setBuffered */
  virtual void setBuffered(bool buffered){}
        
  /** @copydoc Object::capture */
  virtual void capture(){}
        
  /** @copydoc Object::queryInterface */
  virtual OIS::Interface* queryInterface(OIS::Interface::IType type) {return 0;}
        
  /** @copydoc Object::_initialize */
  virtual void _initialize(){}
        
  virtual bool isKeyDown( OIS::KeyCode key ) const{
    return false;
  }
        
  virtual const std::string& getAsString( OIS::KeyCode kc ){
    static std::string defstr = "";
    return defstr;
  }
        
  virtual void copyKeyStates( char keys[256] ) const{
            
  }
};
    
/*=============================================================================
  | Android input injection
  =============================================================================*/
class AndroidInputInjector
{
private:
  CInputListenerManager* mInputListenerManager;
  AndroidMultiTouch* mTouch;
  AndroidKeyboard* mKeyboard;
        
public:
        
  AndroidInputInjector(CInputListenerManager* manager, AndroidMultiTouch* touch, AndroidKeyboard* keyboard) 
    : mInputListenerManager(manager), mTouch(touch), mKeyboard(keyboard) {}
        

  void clearStates() {
    mTouch->clearStates();
  }
  void injectKeyEvent(int action, int32_t keyCode) {
    OIS::KeyCode key = OIS::KC_UNASSIGNED;
    switch (keyCode) {
    case AKEYCODE_BACK:
      key = OIS::KC_ESCAPE;
      break;
    case AKEYCODE_SEARCH:
      key = OIS::KC_RMENU;
      break;
    case AKEYCODE_MENU:
      key = OIS::KC_APPS;
      break;
    case AKEYCODE_DPAD_LEFT:
      key = OIS::KC_LEFT;
      break;
    case AKEYCODE_DPAD_RIGHT:
      key = OIS::KC_RIGHT;
      break;
    case AKEYCODE_DPAD_UP:
      key = OIS::KC_UP;
      break;
    case AKEYCODE_DPAD_DOWN:
      key = OIS::KC_DOWN;
      break;
    case AKEYCODE_DPAD_CENTER:
      key = OIS::KC_RETURN;
      break;
    case AKEYCODE_MEDIA_PLAY_PAUSE:
      key = OIS::KC_PLAY_PAUSE;
      break;
    case KEYCODE_MEDIA_REWIND:
      key = OIS::KC_PREVTRACK;
      break;
    case KEYCODE_MEDIA_FAST_FORWARD:
      key = OIS::KC_NEXTTRACK;
      break;
    }

    OIS::KeyEvent evt(mKeyboard, key, 0);
    if (action == 0) {
      mInputListenerManager->keyPressed(evt);
    }
    else {
      mInputListenerManager->keyReleased(evt);
    }
  }
        
  void injectTouchEvent(int action, float x, float y, int pointerId)
  {
    OIS::MultiTouchState &state = mTouch->getMultiTouchState(pointerId);

    //LOGI("%d x %f y %f id %d", action, x, y, pointerId);
            
    switch(action)
      {
      case 5:
      case 0:
	state.touchType = OIS::MT_Pressed;
	break;
      case 6:
      case 1:
	state.touchType = OIS::MT_Released;
	break;
      case 2:
	state.touchType = OIS::MT_Moved;
	break;
      case 3:
	state.touchType = OIS::MT_Cancelled;
	break;
      default:
	state.touchType = OIS::MT_None;
      }
            
	int last = state.X.abs;
	state.X.abs =  (int)x;
	state.X.rel = state.X.abs - last;
                
	last = state.Y.abs;
	state.Y.abs = (int)y;
	state.Y.rel = state.Y.abs - last;
                
	state.Z.abs = 0;
	state.Z.rel = 0;
    if(state.touchType != OIS::MT_None)
      {
                
	OIS::MultiTouchEvent evt(mTouch, state);
                
	switch(state.touchType)
	  {
	  case OIS::MT_Pressed:
	    mInputListenerManager->touchPressed(evt);
	    break;
	  case OIS::MT_Released:
	    mInputListenerManager->touchReleased(evt);
	    break;
	  case OIS::MT_Moved:
	    mInputListenerManager->touchMoved(evt);
	    break;
	  case OIS::MT_Cancelled:
	    mInputListenerManager->touchCancelled(evt);
	    break;
	  default:
	    break;
	  }
      }
  }
};

#endif
