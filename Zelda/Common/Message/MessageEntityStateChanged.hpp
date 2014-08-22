#ifndef _MESSAGE_ENTITY_STATE_CHANGED_HPP_
#define _MESSAGE_ENTITY_STATE_CHANGED_HPP_

#include "Message.hpp"

#include "../GameLogic/EntityStates.hpp"

class CEntity;

class CMessageEntityStateChanged: public CMessage {
protected:
  const EEntityStateTypes m_eOldState;
  const EEntityStateTypes m_eNewState;

  CEntity &m_Entity;
public:
  CMessageEntityStateChanged(EEntityStateTypes eOldState, EEntityStateTypes eNewState, CEntity &entity);

  EEntityStateTypes getOldState() const {return m_eOldState;}
  EEntityStateTypes getNewState() const {return m_eNewState;}

  CEntity &getEntity() const {return m_Entity;}

};

#endif