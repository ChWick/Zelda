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

#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <string>
#include <vector>
#include <tinyxml2.h>
#include "../../Util/XMLHelper.hpp"
#include "../../GameLogic/OutputStyle.hpp"
#include "../../Config/TypeDefines.hpp"

class ENTITY;

namespace events {

  class CEmitter;
  class CAction;

class CEvent {
private:
  const std::string m_sID;
protected:
  CEntity &m_Owner;
  bool m_bStarted;

  std::vector<CEmitter *> m_lEmitter;
  std::vector<CAction *> m_lActions;

public:
  CEvent(CEntity &owner);
  CEvent(CEntity &owner, const tinyxml2::XMLElement *pElement);

  virtual ~CEvent();

  virtual void init();
  virtual void exit() {}

  void start();
  void stop();

  void update(float tpf);

  const std::string &getID() const {return m_sID;}
  const std::vector<CEmitter *> &getEmitter() const {return m_lEmitter;}
  const std::vector<CAction *> &getActions() const {return m_lActions;}
  CEntity &getOwner() const {return m_Owner;}

  virtual void writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const;

protected:
};

};

#endif
