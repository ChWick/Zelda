#ifndef BOMBLISTENER_H
#define BOMBLISTENER_H

class CBomb;

class CBombListener {
public:
    virtual void bombDestroyed(CBomb *pBomb) = 0;
};

#endif
