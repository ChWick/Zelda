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

#ifndef INNEROBJECTCONTAINERINTERFACE_H
#define INNEROBJECTCONTAINERINTERFACE_H

#include "InnerObjectGenerator.h"

class InnerObjectContainerInterface {
private:
	const inner_object_list_type *m_plInnerObject;    //!< the inner object of this object, may never be NULL set to IOLT_EMPTY instead!)
public:
    InnerObjectContainerInterface();
    virtual ~InnerObjectContainerInterface();

	void setInnerObject(const inner_object_list_type *plInnerObject) {assert(plInnerObject); m_plInnerObject = plInnerObject;}
	const inner_object_list_type &getInnerObject() const {return *m_plInnerObject;}
	CPickableObject *createInnerObject(CMap &map, const Ogre::Vector3 &vPosition);
protected:
private:
};

#endif // INNEROBJECTCONTAINERINTERFACE_H
