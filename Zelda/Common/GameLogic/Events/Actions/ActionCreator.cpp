#include "ActionCreator.hpp"
#include "ActionTypes.hpp"
#include "../../../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "ActionMessage.hpp"

using namespace XMLHelper;

namespace events {
  CAction *createAction(const tinyxml2::XMLElement *pElem, const CEvent &owner) {
    EActionTypes type(ACTION_TYPES_MAP.parseString(Attribute(pElem, "type")));

    switch (type) {
    case ACTION_MESSAGE:
      return new CActionMessage(pElem, owner);
    }

    throw Ogre::Exception(0, "New action type not added in createAction", __FILE__);
  }
};
