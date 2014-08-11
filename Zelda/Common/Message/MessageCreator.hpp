#ifndef _MESSAGE_CREATOR_HPP_
#define _MESSAGE_CREATOR_HPP_

namespace tinyxml2 {
  class XMLElement;
};

class CMessage;

CMessage *createMessage(const tinyxml2::XMLElement *pElem);

#endif // _MESSAGE_CREATOR_HPP_

