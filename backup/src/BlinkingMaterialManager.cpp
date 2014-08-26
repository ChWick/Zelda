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

#include "StdAfx.h"
#include "BlinkingMaterialManager.h"

const Ogre::String BLINKING_MATERIAL_EXTENSION("_Blinking");
template<> CBlinkingMaterialManager* Ogre::Singleton<CBlinkingMaterialManager>::msSingleton = 0;

CBlinkingMaterialManager* CBlinkingMaterialManager::getSingletonPtr(void)
{
    return msSingleton;
}
CBlinkingMaterialManager& CBlinkingMaterialManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
void CBlinkingMaterialManager::createMaterial(const Ogre::String &nonBlinkingMatName) {
	// already created?
	for (auto &mat : m_lMaterials) {
		if (mat.name == nonBlinkingMatName) {
			mat.counter++; // increase counter
			return;
		}
	}

	// create new material
	SMaterialIndex mat;
	mat.counter = 1;
	mat.mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(nonBlinkingMatName))->clone(nonBlinkingMatName + BLINKING_MATERIAL_EXTENSION);
	mat.name = nonBlinkingMatName;
	m_lMaterials.push_back(mat);
}
void CBlinkingMaterialManager::removeMaterial(const Ogre::String &nonBlinkingMatName) {
	// already created?
	for (auto &mat : m_lMaterials) {
		if (mat.name == nonBlinkingMatName) {
			mat.counter--; // increase counter
			if (mat.counter >= 0) {
				// destroy material
				destroyMaterial(mat);
			}
			return;
		}
	}
}
void CBlinkingMaterialManager::destroyMaterial(const SMaterialIndex &mat) {
	Ogre::MaterialManager::getSingleton().remove(mat.mat->getName());

	for (auto it = m_lMaterials.begin(); it != m_lMaterials.end(); it++) {
		if (it->name == mat.name) {
			m_lMaterials.erase(it);
			return;
		}
	}
}
Ogre::MaterialPtr CBlinkingMaterialManager::getMaterial(const Ogre::String &matName) {
	for (auto &mat : m_lMaterials) {
		if (mat.name == matName || mat.mat->getName() == matName) {
			return mat.mat;
		}
	}

	throw Ogre::Exception(0, "Blinking material for " + matName + " not found", __FILE__);
}
Ogre::String CBlinkingMaterialManager::getBlinkingMat(const Ogre::String &matName) {
	assert(matName.find(BLINKING_MATERIAL_EXTENSION) == Ogre::String::npos);

	return matName + BLINKING_MATERIAL_EXTENSION;
}
Ogre::String CBlinkingMaterialManager::getNonBlinkingMat(const Ogre::String &matName) {
	assert(matName.find(BLINKING_MATERIAL_EXTENSION) != Ogre::String::npos);

	return matName.substr(0, matName.length() - BLINKING_MATERIAL_EXTENSION.length());
}
void CBlinkingMaterialManager::update(Ogre::Real tpf) {
	for (auto &mat : m_lMaterials) {
		auto pMat = mat.mat;
		auto diffuse = pMat->getBestTechnique()->getPass(0)->getDiffuse();
		Ogre::Real &red = diffuse[0];
		Ogre::Real &green = diffuse[1];
		red += tpf * 4.5;
		if (red > 1) {red = 0;}
		green -= tpf * 6;
		if (green < 0) {green = 1;}

		pMat->getBestTechnique()->getPass(0)->setDiffuse(diffuse);
	}
}
