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

#ifndef GAME_INPUT_MANAGER_HPP
#define GAME_INPUT_MANAGER_HPP

#include "InputListener.hpp"
#include "GameInputCommand.hpp"
#include <OgreSingleton.h>
#include <list>

class CGameInputListener;
class CGameInputCommand;

class CGameInputManager :
  public CInputListener,
  public Ogre::Singleton<CGameInputManager> {
private:
  std::list<CGameInputListener*> m_lListenerList;       //!< List of the listeners 
  EGameInputState m_InputStates[GIC_COUNT];             //!< Current state of the inputs
public:
  static CGameInputManager& getSingleton(void);
  static CGameInputManager* getSingletonPtr(void);

  CGameInputManager();
  ~CGameInputManager();

  void addInputListener(CGameInputListener *pListener) {
    m_lListenerList.push_back(pListener);
  }
  void removeInputListener(CGameInputListener *pListener) {
    m_lListenerList.remove(pListener);
  }

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );
  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  // OIS::MultiTouchListener
  virtual bool touchMoved(const OIS::MultiTouchEvent& evt);
  virtual bool touchPressed(const OIS::MultiTouchEvent& evt);
  virtual bool touchReleased(const OIS::MultiTouchEvent& evt);
  virtual bool touchCancelled(const OIS::MultiTouchEvent& evt);

  void injectCommand( const CGameInputCommand &cmd );
protected:

  void sendCommandToListeners( const CGameInputCommand &cmd );

};


#endif
