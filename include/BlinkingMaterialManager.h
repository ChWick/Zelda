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

#ifndef _BLINKING_MATERIAL_MANAGER_
#define _BLINKING_MATERIAL_MANAGER_

#include "StdAfx.h"

class CBlinkingMaterialManager : public Ogre::Singleton<CBlinkingMaterialManager> {
protected:
	struct SMaterialIndex {
		Ogre::String name;			//!< name without blinking
		Ogre::MaterialPtr mat;
		unsigned int counter;
	};

	std::list<SMaterialIndex> m_lMaterials;

	void destroyMaterial(const SMaterialIndex &mat);
public:
    static CBlinkingMaterialManager& getSingleton(void);
    static CBlinkingMaterialManager* getSingletonPtr(void);

	void createMaterial(const Ogre::String &nonBlinkingMatName);
	void removeMaterial(const Ogre::String &nonBlinkingMatName);

	Ogre::MaterialPtr getMaterial(const Ogre::String &matName);
	Ogre::String getBlinkingMat(const Ogre::String &matName);
	Ogre::String getNonBlinkingMat(const Ogre::String &matName);

	void update(Ogre::Real tpf);

};

#endif