#ifndef CHARACTERCONTROLLERLISTENER_H
#define CHARACTERCONTROLLERLISTENER_H

class CCharacter;

class CharacterControllerListener {
public:
    virtual void targetReached(CCharacter *pChar) {}
};


#endif
