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