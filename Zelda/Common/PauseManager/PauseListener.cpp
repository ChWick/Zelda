#include "PauseListener.hpp"
#include "PauseManager.hpp"

CPauseListener::CPauseListener() {
  CPauseManager::getSingleton().addListener(this);
}
CPauseListener::~CPauseListener() {
  CPauseManager::getSingleton().removeListener(this);
}
