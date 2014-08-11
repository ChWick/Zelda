#include "Message.hpp"
#include "../Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessage::CMessage(const tinyxml2::XMLElement *pElement)
  : m_Type(MESSAGE_TYPES_MAP.parseString(Attribute(pElement, "message_type"))) {
}
CMessage::CMessage(EMessageTypes type)
  : m_Type(type) {
}
CMessage::~CMessage() {
}
