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

#include "InnerObjectGenerator.hpp"
#include <OgreMath.h>
#include "../../Common/Util/Assert.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using XMLHelper::RealAttribute;
using XMLHelper::Attribute;

namespace InnerObject {

GENERATOR_DATA_LIST EMPTY_DATA_LIST;
CDefaultGeneratorData DEFAULT_GENERATOR_DATA_MAP;

CGenerator::CGenerator(EObjectTypes objectType) {
  SGeneratorData data;
  data.objectType = objectType;
  data.probability = 1;
  mGeneratorDataList.push_back(data);

  ASSERT(consistencyCheck());
}

CGenerator::CGenerator(
    const GENERATOR_DATA_LIST &data)
    : mGeneratorDataList(data) {
  ASSERT(consistencyCheck());
}

EObjectTypes CGenerator::drawRandomObject() const {
  ASSERT(consistencyCheck());

  float random = Ogre::Math::RangeRandom(0, 1);
  float sumProbab = 0;

  for (const SGeneratorData &data : mGeneratorDataList) {
    sumProbab += data.probability;
    if (random < sumProbab) {
      return data.objectType;
    }
  }

  return OBJECT_COUNT;
}

void CGenerator::setObject(EObjectTypes objectType) {
  mGeneratorDataList.clear();
  SGeneratorData data;
  data.objectType = objectType;
  data.probability = 1;
  mGeneratorDataList.push_back(data);

  ASSERT(consistencyCheck());
}

bool CGenerator::consistencyCheck() const {
  float sumProbab = 0;
  for (const SGeneratorData &data : mGeneratorDataList) {
    ASSERT(data.probability >= 0);
    sumProbab += data.probability;
  }

  ASSERT(sumProbab <= 1);
  return true;
}

void CDefaultGeneratorDataLoader::readGroupElement(
    const tinyxml2::XMLElement *e) {
  GENERATOR_DATA_LIST dataList;

  EObjectTypes type = CObjectTypeIdMap::getSingleton().
      parseString(Attribute(e, "id"));

  for (const tinyxml2::XMLElement *c = e->FirstChildElement();
       c; c = c->NextSiblingElement()) {
    ASSERT(strcmp(c->Value(), "data") == 0);

    SGeneratorData data;
    data.probability = RealAttribute(c, "probability");
    data.objectType = CObjectTypeIdMap::getSingleton().parseString(
        Attribute(c, "objectType"));
    dataList.push_back(data);
  }
  DEFAULT_GENERATOR_DATA_MAP.setData(type, dataList);
}

}  // namespace InnerObject

