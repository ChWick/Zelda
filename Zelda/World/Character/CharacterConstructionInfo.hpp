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

#ifndef CHARACTERCONSTRUCTIONINFO_HPP
#define CHARACTERCONSTRUCTIONINFO_HPP

#include "../../Common/World/WorldEntityConstructionInfo.hpp"
#include "CharacterData.hpp"

class CCharacterConstructionInfo
    : public CWorldEntityConstructionInfo {
 private:
  //! class of the character to use on creation
  std::string mCharacterClass;
  //! The animations of the character
  CharacterAnimationDataList mAnimations;
  
 public:
  CCharacterConstructionInfo();
  explicit CCharacterConstructionInfo(const tinyxml2::XMLElement *e);

  virtual void parse(const tinyxml2::XMLElement *e) override;

  CCharacterConstructionInfo &operator=(const CCharacterConstructionInfo&) = default;

  const std::string &getCharacterClass() const {return mCharacterClass;}
  const CharacterAnimationDataList &getAnimationDataList() const {return mAnimations;}
};
  
#endif /* CHARACTERCONSTRUCTIONINFO_HPP */
