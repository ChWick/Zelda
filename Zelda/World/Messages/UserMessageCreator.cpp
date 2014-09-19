#include "UserMessageCreator.hpp"
#include "UserMessageTypes.hpp"
#include "MessageShowText.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessage *CUserMessageCreator::createMessage(const tinyxml2::XMLElement *pElem) {
  unsigned int type(USER_MESSAGE_TYPES_MAP.parseString(Attribute(pElem, m_sMessageTypeAttributeName.c_str())));
  switch (type) {
  case MSG_SHOW_TEXT:
    return new CMessageShowText(pElem);
    break;
  }

  return CMessageCreator::createMessage(pElem);
}
