#include "UserMessageCreator.hpp"
#include "UserMessageTypes.hpp"
#include "MessageShowText.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessage *CUserMessageCreator::createMessage(const tinyxml2::XMLElement *pElem, const Ogre::Any &any) {
  unsigned int type(USER_MESSAGE_TYPES_MAP.parseString(Attribute(pElem, m_sMessageTypeAttributeName.c_str())));
  switch (type) {
  case MSG_SHOW_TEXT:
    return new CMessageShowText(pElem, any.get<std::shared_ptr<CGUITextBox::SResult> >());
    break;
  }

  return CMessageCreator::createMessage(pElem, any);
}
