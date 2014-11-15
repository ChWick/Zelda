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

#ifndef PAUSE_CALLER_HPP
#define PAUSE_CALLER_HPP

#include "PauseTypes.hpp"

class CPauseCaller {
  unsigned int m_uiPausedFlags;
public:
  CPauseCaller();
  ~CPauseCaller();
  void pause(unsigned int uiPauseFlags = PAUSE_ALL);
  void unpause(unsigned int uiPauseFlags = PAUSE_ALL);
  void setPause(unsigned int uiPauseFlags);
  unsigned int getPause() const {return m_uiPausedFlags;}
};

#endif // PAUSE_CALLER_HPP
