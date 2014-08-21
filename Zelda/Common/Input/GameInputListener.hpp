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

#ifndef _GAME_INPUT_LISTENER_HPP_
#define _GAME_INPUT_LISTENER_HPP_

class CGameInputCommand;

class CGameInputListener {
private:
  bool m_bGameInputListenerEnabled;
public:
  CGameInputListener(bool bDefaultEnabled = true);
  ~CGameInputListener();
  virtual void receiveInputCommand(const CGameInputCommand &cmd) = 0;

  void setGameInputListenerEnabled(bool bEnabled) {
    m_bGameInputListenerEnabled = bEnabled;}
  bool isGameInputListenerEnabled() const {
    return m_bGameInputListenerEnabled;}
};

#endif
