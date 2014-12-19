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
#include <memory>
#include <OgreException.h>
#include "Assert.hpp"

template <typename C>
class CEnumIdMapSingleton {
 private:
  static std::unique_ptr<C> mInstance;
 protected:
  CEnumIdMapSingleton() {}
 public:
  virtual ~CEnumIdMapSingleton() {}

  virtual void init() = 0;

  static C &getSingleton() {
    if (!mInstance) {
      mInstance = std::unique_ptr<C>(new C());
    }
    return *mInstance.get();
  }
};

// static variable declaration
template <typename C>
std::unique_ptr<C> CEnumIdMapSingleton<C>::mInstance;

template <typename C, typename T, typename DATA>
class CEnumIdMap
    : public CEnumIdMapSingleton<C> {
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
  const DATA &toData(T t) const {
    ASSERT(m_Map.count(t) == 1);
    return m_Map.at(t);
  }
  const std::map<const T, const DATA> &getData() const {return m_Map;}

  void setData(const T &t, const DATA &data) {
    m_Map[t] = data;
  }
};

template <typename C, typename T>
class CStringEnumIdMap : public CEnumIdMap<C, T, std::string> {
 public:
  // default for using strings
  T parseString(const std::string &str) const {
    for (const std::pair<T, std::string> &p : CEnumIdMap<C, T, std::string>::m_Map) {
      if (p.second == str) {
        return p.first;
      }
    }
    throw Ogre::Exception(0, "'" + str + "' could not be parsed.", __FILE__);
  }
  const std::string &toString(T t) const {
    return CEnumIdMap<C, T, std::string>::toData(t);
  }
};

template <typename T, typename DATA, const DATA &defaultValue>
class CEnumIdMapWithDefaultValue {
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
  const DATA &toData(T t) const {
    auto it = m_Map.find(t);
    if (it == m_Map.end()) {
      return defaultValue;
    }
    return it->second;
  }
  const std::map<const T, const DATA> &getData() const {return m_Map;}

  void setData(const T &t, const DATA &data) {
    m_Map[t] = data;
  }
};

#endif // _ENUM_ID_MAP_HPP_
