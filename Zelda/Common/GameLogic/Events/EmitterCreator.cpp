#include "EmitterCreator.hpp"
#include "EmitterTypes.hpp"
#include "../../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "EmitOnCollision.hpp"

using namespace XMLHelper;

namespace events {
  CEmitter *createEmitter(const tinyxml2::XMLElement *pElem, const CEvent &owner) {
    EEmitterTypes type(EMITTER_TYPES_MAP.parseString(Attribute(pElem, "type")));

    switch (type) {
    case EMIT_ON_COLLISION:
      return new CEmitOnCollision(pElem, owner);
    }

    throw Ogre::Exception(0, "New emitter type not added in createEmitter", __FILE__);
  }
};
