#ifndef PAUSE_LISTENER_HPP
#define PAUSE_LISTENER_HPP

#include "PauseTypes.hpp"

class CPauseListener {
public:
  CPauseListener();
  virtual ~CPauseListener();

  virtual void updatePause(int iPauseType, bool bPause) = 0;
};

#endif // PAUSE_LISTENER_HPP
