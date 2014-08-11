#include "ActionMessage.hpp"
#include "../../../Util/XMLHelper.hpp"
#include "../../../Message/MessageCreator.hpp"
#include "../../../Message/MessageHandler.hpp"

namespace events {

CActionMessage::CActionMessage(const tinyxml2::XMLElement *pElem, const CEvent &owner)
  : CAction(pElem, owner) {

  m_pMessage = createMessage(pElem);
}
CActionMessage::~CActionMessage() {
  delete m_pMessage;
}

void CActionMessage::start() {
  // auto delete to false, since we delete the message on our own!
  CMessageHandler::getSingleton().addMessage(m_pMessage, false);
}

};
