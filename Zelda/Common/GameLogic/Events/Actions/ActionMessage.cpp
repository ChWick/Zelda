#include "ActionMessage.hpp"
#include "../../../Util/XMLHelper.hpp"

namespace events {

CActionMessage::CActionMessage(const tinyxml2::XMLElement *pElem, const CEvent &owner)
  : CAction(pElem, owner) {


}

void CActionMessage::start() {
}

};
