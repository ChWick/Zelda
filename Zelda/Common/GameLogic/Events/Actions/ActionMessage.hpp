#ifndef _ACTION_MESSAGE_HPP_
#define _ACTION_MESSAGE_HPP_

#include "Action.hpp"

namespace events {
  class CActionMessage : public CAction {
  public:
    CActionMessage(const tinyxml2::XMLElement *pElem, const CEvent &owner);

  protected:
    void start();
  };
};

#endif // _ACTION_MESSAGE_HPP_
