#ifndef _MESSAGE_ENTITY_DATA_CHANGED_HPP_
#define _MESSAGE_ENTITY_DATA_CHANGED_HPP_

#include "Message.hpp"

class CEntityProperty;

class CMessageEntityDataChanged : public CMessage {
public:
protected:
const CEntityProperty &mProperty;

public:
CMessageEntityDataChanged(const CEntityProperty &property)
  : CMessage(MSG_ENTITY_DATA_CHANGED), 
    mProperty(property) {
}
  
  const CEntityProperty &getProperty() const {return mProperty;}
};

#endif
