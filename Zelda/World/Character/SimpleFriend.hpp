/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef SIMPLEFRIEND_H
#define SIMPLEFRIEND_H

#include "Person.hpp"

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
    SimpleFriend(const std::string &sID, CEntity *pParent, CMap *pMap, ESimpleFriendTypes eType);
    virtual ~SimpleFriend();

protected:
	void setupInternal();
  void setupAnimations();
private:
};

#endif // SIMPLEFRIEND_H
