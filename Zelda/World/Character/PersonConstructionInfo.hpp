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

#ifndef PERSONCONSTRUCTIONINFO_HPP
#define PERSONCONSTRUCTIONINFO_HPP

#include <OgreVector3.h>
#include "../../Common/World/Hitpoints.hpp"
#include "CharacterConstructionInfo.hpp"
#include "PersonTypes.hpp"

class CPersonConstructionInfo
    : public CCharacterConstructionInfo {
 private:
  EPersonTypes mPersonType;
  std::string mMeshName;
  std::string mMaterialName;
  Ogre::Vector3 mScale;
  
 public:
  CPersonConstructionInfo();

  virtual void parse(const tinyxml2::XMLElement *e) override;

  CPersonConstructionInfo &operator=(const CPersonConstructionInfo &) = default;

  EPersonTypes getPersonType() const {return mPersonType;}
  const std::string &getMeshName() const {return mMeshName;}
  const std::string &getMaterialName() const {return mMaterialName;}
  const Ogre::Vector3 &getScale() const {return mScale;}
};

#endif /* PERSONCONSTRUCTIONINFO_HPP */
