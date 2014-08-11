#include "MessageCreator.hpp"
#include "MessageTypes.hpp"
#include "../Util/XMLHelper.hpp"
#include <OgreException.h>

#include "MessageSwitchMap.hpp"

using namespace XMLHelper;

CMessage *createMessage(const tinyxml2::XMLElement *pElem) {
  EMessageTypes type(MESSAGE_TYPES_MAP.parseString(Attribute(pElem, "message_type")));

  switch (type) {
  case MSG_SWITCH_MAP:
    return new CMessageSwitchMap(pElem);
  }

  throw Ogre::Exception(0, "New message type not added in createMessage", __FILE__);
}
