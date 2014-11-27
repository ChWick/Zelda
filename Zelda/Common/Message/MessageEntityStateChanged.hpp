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

#ifndef _MESSAGE_ENTITY_STATE_CHANGED_HPP_
#define _MESSAGE_ENTITY_STATE_CHANGED_HPP_

#include "Message.hpp"

#include "../GameLogic/EntityStates.hpp"

class CEntity;

class CMessageEntityStateChanged: public CMessage {
protected:
  const EEntityStateTypes m_eOldState;
  const EEntityStateTypes m_eNewState;

  CEntity *m_Entity;
public:
  CMessageEntityStateChanged(EEntityStateTypes eOldState, EEntityStateTypes eNewState, CEntity *entity);

  EEntityStateTypes getOldState() const {return m_eOldState;}
  EEntityStateTypes getNewState() const {return m_eNewState;}

  CEntity *getEntity() const {return m_Entity;}

};

#endif
