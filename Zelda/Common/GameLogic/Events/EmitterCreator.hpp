#ifndef _EMITTER_CREATOR_HPP_
#define _EMITTER_CREATOR_HPP_

namespace tinyxml2 {
  class XMLElement;
};

namespace events {
  class CEmitter;

  CEmitter *createEmitter(const tinyxml2::XMLElement *pElem);
};

#endif // _EMITTER_CREATOR_HPP_
