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

#ifndef _ENUM_ID_MAP_HPP_
#define _ENUM_ID_MAP_HPP_

#include <map>
#include <string>
#include <OgreException.h>

template <typename T, typename DATA = std::string>
class CEnumIdMap {
protected:
  std::map<T, DATA> m_Map;

public:
  // for using custom data
  T parseData(const DATA &str) const {
    for (const std::pair<T, std::string> &p : m_Map) {
      if (p.second == str) {
        return p.first;
      }
    }
    throw Ogre::Exception(0, "Data could not be parsed.", __FILE__);
  }
  const DATA &toData(T t) const {return m_Map.at(t);}
  const std::map<const T, const DATA> &getData() const {return m_Map;}

  // default for using strings
  T parseString(const DATA &str) const {
    for (const std::pair<T, std::string> &p : m_Map) {
      if (p.second == str) {
        return p.first;
      }
    }
    throw Ogre::Exception(0, "'" + str + "' could not be parsed.", __FILE__);
  }
  const DATA &toString(T t) const {return m_Map.at(t);}
};
#endif // _ENUM_ID_MAP_HPP_
