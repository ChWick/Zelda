#ifndef SIMPLEFRIEND_H
#define SIMPLEFRIEND_H

#include "Person.h"

//! Class for simple friends (characters)
/**
  * Like father of link or zelda, ...
  */
class SimpleFriend : public CPerson {
public:
    //! List of the characters
    enum ESimpleFriendTypes {
        SF_LINKS_FATHER,                //!< father of link
    };
private:
    ESimpleFriendTypes m_eType;         //!< type of the friend
public:
    SimpleFriend(const std::string &sID, CEntity *pParent, ESimpleFriendTypes eType, CMap *pMap);
    virtual ~SimpleFriend();

protected:
	void setupInternal();
private:
};

#endif // SIMPLEFRIEND_H
