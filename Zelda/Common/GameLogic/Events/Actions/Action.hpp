#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include "ActionTypes.hpp"

namespace tinyxml2 {
  class XMLElement;
};

namespace events {
  class CEvent;

  class CAction {
  protected:
    const EActionTypes m_Type;
    const CEvent &m_Owner;

    CAction(const tinyxml2::XMLElement *pElem, const CEvent &owner);
    virtual ~CAction() {}

  public:
    virtual void start() {}
    virtual void stop() {}
  };
};
#endif // _ACTION_HPP_
