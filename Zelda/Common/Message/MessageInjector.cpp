#include "MessageInjector.hpp"
#include "MessageHandler.hpp"

CMessageInjector::CMessageInjector() {
  if (CMessageHandler::getSingletonPtr()) {
    CMessageHandler::getSingleton().addInjector(this);
  }
}
CMessageInjector::~CMessageInjector() {
  if (CMessageHandler::getSingletonPtr()) {
    CMessageHandler::getSingleton().removeInjector(this);
  }
}
