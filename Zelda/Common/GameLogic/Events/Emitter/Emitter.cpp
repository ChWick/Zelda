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

#include "Emitter.hpp"
#include "EmitterConstructionInfo.hpp"
#include "../../../tinyxml2/tinyxml2.hpp"
#include "../../../Util/XMLHelper.hpp"

using XMLHelper::Attribute;

namespace events {

CEmitter::CEmitter(EEmitterTypes eType, const CEvent &owner)
    : m_eType(eType),
      m_Owner(owner) {
}

CEmitter::CEmitter(const tinyxml2::XMLElement *pElem, const CEvent &owner)
      : m_eType(CEmitterTypesMap::getSingleton().parseString(
            Attribute(pElem, "type"))),
        m_Owner(owner) {
}

CEmitter::CEmitter(const std::shared_ptr<CEmitterConstructionInfo> info,
                   const CEvent &owner)
    : m_eType(info->getType()),
      m_Owner(owner) {
}

}  // namespace events
