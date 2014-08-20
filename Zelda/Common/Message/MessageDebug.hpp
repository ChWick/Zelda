#ifndef _MESSAGE_DEBUG_HPP_
#define _MESSAGE_DEBUG_HPP_

#include "Message.hpp"



class CMessageDebug: public CMessage {
public:
  enum EDebugMessageTypes {
    DM_TOGGLE_PHYSICS,
  };
protected:
  const EDebugMessageTypes m_eDebugType;
  bool m_bActive;
public:
  CMessageDebug(const EDebugMessageTypes type, bool bActive);

  EDebugMessageTypes getDebugType() const {return m_eDebugType;}

  void setActive(bool bActive) {m_bActive = bActive;}
  bool isActive() const {return m_bActive;}

};

#endif // _MESSAGE_TARGET_REACHED_HPP_
