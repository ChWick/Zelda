#ifndef PAUSE_CALLER_HPP
#define PAUSE_CALLER_HPP

#include "PauseTypes.hpp"

class CPauseCaller {
  unsigned int m_uiPausedFlags;
public:
  CPauseCaller();
  ~CPauseCaller();
  void pause(unsigned int uiPauseFlags = PAUSE_ALL);
  void unpause(unsigned int uiPauseFlags = PAUSE_ALL);
  void setPause(unsigned int uiPauseFlags);
  unsigned int getPause() const {return m_uiPausedFlags;}
};

#endif // PAUSE_CALLER_HPP
