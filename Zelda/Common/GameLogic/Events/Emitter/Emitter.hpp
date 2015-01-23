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

#ifndef _EMITTER_HPP_
#define _EMITTER_HPP_

#include "EmitterTypes.hpp"

namespace tinyxml2 {
class XMLElement;
}  // namespace tinyxml2

namespace events {
class CEvent;
class CEmitterConstructionInfo;

class CEmitter {
 protected:
  const EEmitterTypes m_eType;
  const CEvent &m_Owner;
  
 public:
  CEmitter(EEmitterTypes eType, const CEvent &owner);
  CEmitter(const std::shared_ptr<CEmitterConstructionInfo> info,
           const CEvent &owner);
  
  CEmitter(const tinyxml2::XMLElement *pElem, const CEvent &owner);
  virtual ~CEmitter() {}
  
  virtual bool isFiring(float tpf) {return false;}
  
  EEmitterTypes getType() {return m_eType;}
};

}  // namespace events
#endif // _EMITTER_HPP_
