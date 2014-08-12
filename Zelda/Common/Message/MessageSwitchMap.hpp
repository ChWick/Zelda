#ifndef _MESSAGE_SWITCH_MAP_HPP_
#define _MESSAGE_SWITCH_MAP_HPP_

#include "Message.hpp"
#include <string>

class CMap;

class CMessageSwitchMap : public CMessage {
public:
  enum ESwitchMapStatus {
    INJECT,
    SWITCHING,
    FINISHED,
  };
protected:
  const std::string m_sMap;
  ESwitchMapStatus m_eStatus;

  const CMap *m_pFromMap;
  const CMap *m_pToMap;
public:
  CMessageSwitchMap(const tinyxml2::XMLElement *pElem);
  CMessageSwitchMap(const std::string &sMap,
                    ESwitchMapStatus eStatus,
                    const CMap *pFromMap,
                    const CMap *pToMap);

  const std::string &getMap() const {return m_sMap;}
  const ESwitchMapStatus getStatus() const {return m_eStatus;}
  const CMap *getFromMap() const {return m_pFromMap;}
  const CMap *getToMap() const {return m_pToMap;}
};

#endif // _MESSAGE_SWITCH_MAP_HPP_
