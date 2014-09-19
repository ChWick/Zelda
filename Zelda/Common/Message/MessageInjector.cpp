#include "MessageInjector.hpp"
#include "MessageHandler.hpp"

CMessageInjector::CMessageInjector(bool bAutoAddAsInjector) {
  if (bAutoAddAsInjector && CMessageHandler::getSingletonPtr()) {
    CMessageHandler::getSingleton().addInjector(this);
  }
}
CMessageInjector::~CMessageInjector() {
  if (CMessageHandler::getSingletonPtr()) {
    CMessageHandler::getSingleton().removeInjector(this);
  }
}
