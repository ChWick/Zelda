#ifndef _MESSAGE_TARGET_REACHED_HPP_
#define _MESSAGE_TARGET_REACHED_HPP_

#include "Message.hpp"
#include "../Config/TypeDefines.hpp"

class ENTITY;

class CMessageTargetReached: public CMessage {
protected:
  ENTITY *m_pEntity;

public:
  CMessageTargetReached(ENTITY *pEntity);

  ENTITY *getEntity() const {return m_pEntity;}
};

#endif // _MESSAGE_TARGET_REACHED_HPP_
