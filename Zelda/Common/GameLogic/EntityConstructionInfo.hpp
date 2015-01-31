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

#ifndef _ENTITY_CONSTRUCTION_INFO_HPP_
#define _ENTITY_CONSTRUCTION_INFO_HPP_

#include <vector>
#include <memory>
#include <OgreNameGenerator.h>
#include "EntityStates.hpp"
#include "Events/EventConstructionInfo.hpp"

typedef std::vector<
  std::shared_ptr<events::CEventConstructionInfo> > EventConstructionInfoList;

class CEntityConstructionInfo {
 private:
  static Ogre::NameGenerator mNameGenerator;
  
  std::string mID;
  std::string mResourceGroup;
  uint16_t mType;
  EEntityStateTypes mState;
  bool mPauseRender;
  bool mPauseUpdate;
  
  EventConstructionInfoList mEventConstructionInfos;
 public:
  //! empty construction info will use a generated id
  CEntityConstructionInfo();
  //! default constructor requires at least an id
  CEntityConstructionInfo(const std::string &id);

  //! parse from a tinyxml element
  virtual void parse(const tinyxml2::XMLElement *e);

  CEntityConstructionInfo &operator=(const CEntityConstructionInfo &) = default;

  const std::string &getID() const {return mID;}
  void setID(const std::string &id) {mID = id;}

  const std::string &getResourceGroup() const {return mResourceGroup;}
  void setResourceGroup(const std::string &rg) {mResourceGroup = rg;}

  uint16_t getType() const {return mType;}
  void setType(const uint16_t &t) {mType = t;}

  EEntityStateTypes getState() const {return mState;}
  void setState(const EEntityStateTypes est) {mState = est;}

  bool isPauseRender() const {return mPauseRender;}
  void setPauseRender(bool p) {mPauseRender = p;}

  bool isPauseUpdate() const {return mPauseUpdate;}
  void setPauseUpdate(bool p) {mPauseUpdate = p;}

  const EventConstructionInfoList &getEventConstructionList() const {
    return mEventConstructionInfos;
  }
  EventConstructionInfoList &getEventConstructionList() {
    return mEventConstructionInfos;
  }
};

#endif /* _ENTITY_CONSTRUCTION_INFO_HPP_ */
