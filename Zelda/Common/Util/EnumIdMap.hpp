#ifndef _ENUM_ID_MAP_HPP_
#define _ENUM_ID_MAP_HPP_

#include <map>
#include <string>
#include <OgreException.h>

template <typename T>
class CEnumIdMap {
protected:
  std::map<T, std::string> m_Map;

public:

  T parseString(const std::string &str) const {
    for (const std::pair<T, std::string> &p : m_Map) {
      if (p.second == str) {
        return p.first;
      }
    }
    throw Ogre::Exception(0, "'" + str + "' could not be parsed.", __FILE__);
  }
  std::string toString(T t) const {return m_Map.at(t);}
};
#endif // _ENUM_ID_MAP_HPP_
