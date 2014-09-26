#include "MessageShowText.hpp"
#include "UserMessageTypes.hpp"
#include "../../Common/Util/XMLHelper.hpp"

using namespace XMLHelper;


CMessageShowText::CStatusIDMap::CStatusIDMap() {
  m_Map[REQUEST] = "request";
  m_Map[FINISHED] = "finished";
  m_Map[FINISHED_RESULT_CONTINUE] = "finished_result_continue";
  m_Map[FINISHED_RESULT_REPEAT] = "finished_result_repeat";
}

const CMessageShowText::CStatusIDMap STATUS_ID_MAP;

CMessageShowText::CMessageShowText(const std::string &sLanguageString, std::shared_ptr<CGUITextBox::SResult> result, EStatus status)
  : CMessage(MSG_SHOW_TEXT),
    m_sLanguageString(sLanguageString),
    mStatus(status),
    mResult(result) {
}

CMessageShowText::CMessageShowText(const tinyxml2::XMLElement *pElem, std::shared_ptr<CGUITextBox::SResult> result)
  : CMessage(MSG_SHOW_TEXT, pElem),
    m_sLanguageString(Attribute(pElem, "language_string")),
    mStatus(STATUS_ID_MAP.parseString(Attribute(pElem, "status", STATUS_ID_MAP.toString(REQUEST).c_str()))),
    mResult(result) {
}
