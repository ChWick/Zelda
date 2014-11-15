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

#ifndef _GAME_MEMORY_HPP_
#define _GAME_MEMORY_HPP_

#include <OgreSingleton.h>
#include <mutex>
#include <map>
#include <string>

template <typename T>
class CGameMemoryData {
private:
  std::map<std::string, T> mData;
  mutable std::mutex mDataAccessMutex;

public:
  const T &getData(const std::string &id, const T &defaultValue) {
    std::lock_guard<std::mutex> lock(mDataAccessMutex);

    auto i = mData.find(id);
    if (i == mData.end()) {
      mData[id] = defaultValue;
      return defaultValue;
    }
    return mData.at(id);
  }
  void setData(const std::string &id, const T &value) {
    std::lock_guard<std::mutex> lock(mDataAccessMutex);

    mData[id] = value;
  }
};

//! The game memory is a global memory accessable by everyone
/**
  * It is used if different parts of the game have to share data
  * E.g. if a map is deleted but gives some data to the next one
  */
class CGameMemory : public Ogre::Singleton<CGameMemory> {
private:
  CGameMemoryData<int> mIntData;
  CGameMemoryData<Ogre::Real> mRealData;
public:
  static CGameMemory &getSingleton();
  static CGameMemory *getSingletonPtr();

  int getIntData(const std::string &id, const int defaultValue = 0) {return mIntData.getData(id, defaultValue);}
  int setIntData(const std::string &id, const int value) {mIntData.setData(id, value);}

  Ogre::Real getRealData(const std::string &id, const Ogre::Real defaultValue = 0) {return mRealData.getData(id, defaultValue);}
  Ogre::Real setRealData(const std::string &id, const Ogre::Real value) {mRealData.setData(id, value);}
};

#endif // _GAME_MEMORY_HPP_
