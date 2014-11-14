#include "EntityProperty.hpp"
#include "../Message/MessageHandler.hpp"
#include "../Message/MessageEntityDataChanged.hpp"

void CEntityProperty::dataChanged() {
  CMessageHandler::getSingleton().addMessage(new CMessageEntityDataChanged(*this));
}
