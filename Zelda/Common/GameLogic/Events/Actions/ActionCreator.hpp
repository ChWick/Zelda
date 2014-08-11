#ifndef _ACTION_CREATOR_HPP_
#define _ACTION_CREATOR_HPP_

namespace tinyxml2 {
  class XMLElement;
};

namespace events {
  class CEvent;
  class CAction;

  CAction *createAction(const tinyxml2::XMLElement *pElem, const CEvent &owner);
};

#endif // _ACTION_CREATOR_HPP_

