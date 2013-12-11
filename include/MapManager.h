#pragma once

#include "Fader.h"
#include "OgreSingleton.h"
#include "MapLocalisationData.h"

class CPhysicsManager;
class CMapDescription;
class CDungeon;
class CGame;
class CPlayer;
class CCameraController;
class CMap;

class CMapManager : public Ogre::Singleton<CMapManager>, public FaderCallback {
public:
	enum EMaterialFlags {
		MF_NONE,
		MF_TWO_SIDED,
		MF_ALPHA_FROM_TEXTURE,
	};
	enum EPaletteTypes {
		PT_DEFAULT,					//!< Alles pink
		PT_WET_CELLAR,				//!< Lilanes Ambiente
		PT_DOMICILE,				//!< Inneres eines Hauses
		PT_COUNT,
	};
private:
	CMap *m_pMainMap;			//!< Store this extra, since loadign is durable and user can switch often (small "dungeons", caves)
	CMap *m_pCurrentMap;		//!< Stores the current active map, may be m_pMainMap
	CPlayer *m_pPlayer;			//!< Player
	Ogre::SceneManager *m_pSceneManager;
	Ogre::SceneNode *m_pMapSceneNode;
	CCameraController *m_pCameraController;	//!< The camera controller

	//Ogre::map<Ogre::String, Ogre::MaterialPtr>::type m_MaterialMap;

	//CDungeon *m_pCurrentDungeon;

	Fader m_Fader;

	MapLocalisationData m_NextMapLocalisationData;
	Ogre::String m_NextId;

	//CPalette m_Palettes[PT_COUNT];
public:
    static CMapManager& getSingleton(void);
    static CMapManager* getSingletonPtr(void);
	CMapManager(Ogre::SceneManager *pSceneManager, Ogre::SceneNode *pParentSceneNode);
	~CMapManager();

	void init();

	void restartMapLuaScript();

	void update(const Ogre::Real &fTime);

	void changeMap(const MapLocalisationData &locData, const Ogre::String &enterId);
	void destroyMap();

	CMap *getCurrentMap() {return m_pCurrentMap;}

	Ogre::SceneManager *getRootSceneManager() {return m_pSceneManager;}
	CCameraController *getCameraController() const {return m_pCameraController;}

     void fadeInCallback(void);
     void fadeOutCallback(void);

	 // Functions to DEBUG
	 void togglePhysicsDebug();

private:
};
