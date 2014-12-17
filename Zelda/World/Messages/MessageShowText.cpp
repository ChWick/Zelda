/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

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

CMessageShowText::CMessageShowText(const std::string &creationFile,
                                   const std::string &sLanguageString,
                                   std::shared_ptr<CGUITextBox::SResult> result,
                                   EStatus status)
    : CMessage(creationFile, MSG_SHOW_TEXT),
      m_sLanguageString(sLanguageString),
      mStatus(status),
      mResult(result) {
}

CMessageShowText::CMessageShowText(const std::string &creationFile,
                                   const tinyxml2::XMLElement *pElem,
                                   std::shared_ptr<CGUITextBox::SResult> result)
    : CMessage(creationFile, MSG_SHOW_TEXT, pElem),
      m_sLanguageString(Attribute(pElem, "language_string")),
      mStatus(STATUS_ID_MAP.parseString(Attribute(pElem, "status", STATUS_ID_MAP.toString(REQUEST).c_str()))),
      mResult(result) {
}
