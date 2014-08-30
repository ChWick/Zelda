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

#include "ActionDeleteObject.hpp"
#include "../../../Util/XMLHelper.hpp"
#include "../Event.hpp"
#include "../../Entity.hpp"
#include "../../../Log.hpp"

using namespace XMLHelper;

namespace events {

CActionDeleteObject::CActionDeleteObject(const tinyxml2::XMLElement *pElem, const CEvent &owner)
  : CAction(pElem, owner),
    m_sID(Attribute(pElem, "id")),
    m_pEntity(nullptr) {

}

CActionDeleteObject::CActionDeleteObject(CEntity *pEntity, const CEvent &owner)
  : CAction(ACTION_DELETE_OBJECT, owner),
    m_sID(pEntity->getID()),
    m_pEntity(pEntity) {
}

CActionDeleteObject::~CActionDeleteObject() {
}

void CActionDeleteObject::start() {
  if (m_pEntity) {
    m_pEntity->deleteLater();
  }
  else {
    // get from id
    LOGI("Has not be implemented! %s", __FILE__);
  }
}

};
