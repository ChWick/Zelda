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

#ifndef _EVENT_CONSTRUCTION_INFO_HPP_
#define _EVENT_CONSTRUCTION_INFO_HPP_

#include <vector>
#include <memory>
#include <string>

#include "RepeatTypes.hpp"
#include "Emitter/EmitterConstructionInfo.hpp"
#include "Actions/ActionConstructionInfo.hpp"

namespace tinyxml2 {
class XMLElement;
class XMLDocument;
};


namespace events {

typedef std::vector<
  std::shared_ptr<CEmitterConstructionInfo> > EmitterConstructionInfoList;
typedef std::vector<
  std::shared_ptr<CActionConstructionInfo> > ActionConstructionInfoList;

class CEventConstructionInfo {
 private:
  std::string mID;
  ERepeatTypes mRepeatType;
  Ogre::Real mRepeatTime;
  Ogre::Real mTimer;
  bool mStarted;
  
  EmitterConstructionInfoList mEmitterConstructionInfos;
  ActionConstructionInfoList mActionConstructionInfos;
 public:
  //! default constructor
  CEventConstructionInfo();

  //! constructor from a tinyxml element
  CEventConstructionInfo(const tinyxml2::XMLElement *e);

  const std::string &getID() const {return mID;}
  void setID(const std::string &id) {mID = id;}

  ERepeatTypes getRepeatType() const {return mRepeatType;}
  void setRepeatType(ERepeatTypes rt) {mRepeatType = rt;}

  const Ogre::Real &getRepeatTime() const {return mRepeatTime;}
  void setRepeatTime(const Ogre::Real &t) {mRepeatTime = t;}

  const Ogre::Real &getTimer() const {return mTimer;}
  void setTimer(const Ogre::Real &t) {mTimer = t;}

  bool isStarted() const {return mStarted;}
  void setStarted(bool b) {mStarted = b;}

  const EmitterConstructionInfoList &getEmitterInfos() const {
    return mEmitterConstructionInfos;
  }
  EmitterConstructionInfoList &getEmitterInfos() {
    return mEmitterConstructionInfos;
  }

  const ActionConstructionInfoList &getActionInfos() const {
    return mActionConstructionInfos;
  }
  ActionConstructionInfoList &getActionInfos() {
    return mActionConstructionInfos;
  }
};

}  // namespace events

#endif /* _EVENT_CONSTRUCTION_INFO_HPP_ */
