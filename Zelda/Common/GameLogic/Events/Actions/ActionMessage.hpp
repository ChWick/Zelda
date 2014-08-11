#ifndef _ACTION_MESSAGE_HPP_
#define _ACTION_MESSAGE_HPP_

#include "Action.hpp"

class CMessage;

namespace events {
  class CActionMessage : public CAction {
  protected:
    CMessage *m_pMessage;
  public:
    CActionMessage(const tinyxml2::XMLElement *pElem, const CEvent &owner);
    ~CActionMessage();

  protected:
    void start();
  };
};

#endif // _ACTION_MESSAGE_HPP_
