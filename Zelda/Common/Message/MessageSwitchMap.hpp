#ifndef _MESSAGE_SWITCH_MAP_HPP_
#define _MESSAGE_SWITCH_MAP_HPP_

#include "Message.hpp"
#include <string>

class CMessageSwitchMap : public CMessage {
protected:
  const std::string m_sMap;
public:
  CMessageSwitchMap(const tinyxml2::XMLElement *pElem);

  const std::string &getMap() const {return m_sMap;}
};

#endif // _MESSAGE_SWITCH_MAP_HPP_
