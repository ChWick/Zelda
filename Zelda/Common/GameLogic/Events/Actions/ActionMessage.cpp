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

#include "ActionMessage.hpp"
#include "../../../Util/XMLHelper.hpp"
#include "../../../Message/MessageCreator.hpp"
#include "../../../Message/MessageHandler.hpp"

namespace events {

CActionMessage::CActionMessage(const tinyxml2::XMLElement *pElem, const CEvent &owner)
  : CAction(pElem, owner) {

  m_pMessage = createMessage(pElem);
}
CActionMessage::~CActionMessage() {
  delete m_pMessage;
}

void CActionMessage::start() {
  // auto delete to false, since we delete the message on our own!
  CMessageHandler::getSingleton().addMessage(m_pMessage, false);
}

};
