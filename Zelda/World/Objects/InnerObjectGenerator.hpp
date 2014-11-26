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

#ifndef INNEROBJECTGENERATOR_HPP
#define INNEROBJECTGENERATOR_HPP

#include <vector>
#include "ObjectTypes.hpp"
#include "../../Common/Util/XMLLoader.hpp"

namespace InnerObject {

struct SGeneratorData {
  float probability;           //!< Probability to occur [0,1]
  EObjectTypes objectType;     //!< Object to generate
};

typedef std::vector<SGeneratorData> GENERATOR_DATA_LIST;
class CGenerator {
 public:
 private:
  GENERATOR_DATA_LIST mGeneratorDataList;
 public:
  CGenerator(EObjectTypes objectType = OBJECT_COUNT);
  CGenerator(const GENERATOR_DATA_LIST &data);

  EObjectTypes drawRandomObject() const;

  void setObject(EObjectTypes objectType);
 private:
  bool consistencyCheck() const;
};

extern GENERATOR_DATA_LIST EMPTY_DATA_LIST;

class CDefaultGeneratorData
    : public CEnumIdMapWithDefaultValue<EObjectTypes, GENERATOR_DATA_LIST, EMPTY_DATA_LIST> {
};

extern CDefaultGeneratorData DEFAULT_GENERATOR_DATA_MAP;

class CDefaultGeneratorDataLoader : public CXMLLoaderGroup {
 public:
  virtual void readGroupElement(const tinyxml2::XMLElement *e);
};

}  // namespace InnerObject

#endif /* INNEROBJECTGENERATOR_HPP */
