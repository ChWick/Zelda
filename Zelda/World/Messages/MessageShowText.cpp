#include "MessageShowText.hpp"
#include "UserMessageTypes.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;

CMessageShowText::CMessageShowText(const std::string &sLanguageString)
  : CMessage(MSG_SHOW_TEXT),
    m_sLanguageString(sLanguageString) {
}

CMessageShowText::CMessageShowText(const tinyxml2::XMLElement *pElem)
  : CMessage(MSG_SHOW_TEXT, pElem),
    m_sLanguageString(Attribute(pElem, "language_string")) {
}
