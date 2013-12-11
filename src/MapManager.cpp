#include "StdAfx.h"
#include "MapManager.h"
#include "PhysicsManager.h"
#include "DotsceneLoader.h"
#include "MainMap.h"
#include "Player.h"
#include "InputListener.h"
#include "CameraController.h"
#include "GameState.h"
//#include "Map.h"
//#include "Dungeon.h"
//#include "Player.h"
#include "HUD.h"
#include "GameScriptParser.h"

MapLocalisationData MAIN_MAP_LOC_DATA;

template<> CMapManager* Ogre::Singleton<CMapManager>::msSingleton = 0;

CMapManager* CMapManager::getSingletonPtr(void)
{
    return msSingleton;
}
CMapManager& CMapManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

CMapManager::CMapManager(Ogre::SceneManager *pSceneManager, Ogre::SceneNode *pParentSceneNode)
: m_pSceneManager(pSceneManager), m_pMainMap(NULL), m_pCurrentMap(NULL), m_Fader("Overlays/FadeInOut", "Default/OverlayMaterial", this), m_pPlayer(NULL) {
	new CGameState();
	m_pCameraController = new CCameraController(pSceneManager->getCamera("GameCam"), pParentSceneNode);
	CGameState::getSingleton().addCallback(m_pCameraController);

	new GameScriptParser();
}
CMapManager::~CMapManager() {
    if (GameScriptParser::getSingletonPtr()) {delete GameScriptParser::getSingletonPtr();}

	if (m_pPlayer) {
		m_pPlayer->destroy();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	if (m_pCurrentMap) {
		m_pCameraController->leaveMap();
	}
	if (m_pMainMap) {
		if (m_pMainMap == m_pCurrentMap) {
			// dont delete twice
			m_pCurrentMap = NULL;
		}
		m_pMainMap->unload();
		delete m_pMainMap;
		m_pMainMap = NULL;
	}
	if (m_pCurrentMap) {
		m_pCurrentMap->unload();
		delete m_pCurrentMap;
		m_pCurrentMap = NULL;
	}
	if (m_pCameraController) {
		delete m_pCameraController;
		m_pCameraController = NULL;
	}
	destroyMap();
	delete CGameState::getSingletonPtr();
}
void CMapManager::init() {
	// read entry point from file
	// one can make this code beatyful ;-), its fast coded, and not nice!
	std::ifstream entryPointFile("entryPoint.txt");
	if (!entryPointFile.is_open()) {
		throw Ogre::Exception(0, "entryPoint.txt could not be opened", __FILE__);
	}
	MapLocalisationData locData;
	Ogre::String entryPoint;
	while (!entryPointFile.eof()) {
		std::string line;
		std::getline(entryPointFile, line);
		if (line.find("#") == 0 || line.empty()) {continue;}
		std::vector<std::string> splitted;
		boost::split(splitted, line, boost::is_any_of(" "), boost::token_compress_on);
		if (splitted.size() != 3) {
			throw Ogre::Exception(1, "entryPoint: you have to specify 2 arguments (map path and entry point)", __FILE__);
		}

		if (splitted[0] == "DEFINE_MAIN_MAP") {
			MAIN_MAP_LOC_DATA.m_sPath = splitted[1];
			MAIN_MAP_LOC_DATA.m_sName = splitted[2];
			continue;
		}

		locData.m_sPath = splitted[0];
		locData.m_sName = splitted[1];
		entryPoint = splitted[2];
		if (locData.m_sPath == "MAIN_MAP_PATH") {
			locData.m_sPath = MAIN_MAP_LOC_DATA.m_sPath;
		}
		if (locData.m_sName == "MAIN_MAP_NAME") {
			locData.m_sName = MAIN_MAP_LOC_DATA.m_sName;
		}
		break;
	}
	if (locData.m_sName.empty() || locData.m_sPath.empty() || entryPoint.empty()) {
		throw Ogre::Exception(2, "entryPoint: you have to define mapname and entry point", __FILE__);
	}

	/*auto celShadingVP = Ogre::GpuProgramManager::getSingleton().createProgram("Ogre/CelShadingTexturedVP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "Example_CelShadingTextured.cg", Ogre::GPT_VERTEX_PROGRAM, "vs_1_1 arbvp1");
	auto celShadingFP = Ogre::GpuProgramManager::getSingleton().createProgram("Ogre/CelShadingTexturedFP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "Example_CelShadingTextured.cg", Ogre::GPT_FRAGMENT_PROGRAM, "ps_2_0 arbfp1 fp20");
	Ogre::GpuProgramManager::getSingleton().createProgram("Ogre/CelShadingVP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "Example_CelShading.cg", Ogre::GPT_VERTEX_PROGRAM, "vs_4_0 vs_1_1 arbvp1");
	Ogre::GpuProgramManager::getSingleton().createProgram("Ogre/CelShadingFP", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "Example_CelShading.cg", Ogre::GPT_FRAGMENT_PROGRAM, "ps_4_0 ps_1_1 arbfp1 fp20");
*/
	// enter the map
	changeMap(locData, entryPoint);
}
void CMapManager::restartMapLuaScript() {
    // reparse script for possible changes
	GameScriptParser::getSingleton().restart();
}
void CMapManager::update(const Ogre::Real &fTime) {
	if (m_pCurrentMap) {
		m_pCurrentMap->update(fTime);
	}
	if (m_pPlayer) {
		m_pPlayer->update(fTime);
	}
	m_pCameraController->updateCamera(fTime);
	m_Fader.fade(fTime);
}
void CMapManager::destroyMap() {
	/*if (!m_pCurrentDungeon) {return;}
	delete m_pCurrentDungeon;
	m_pCurrentDungeon = 0;*/
}
void CMapManager::changeMap(const MapLocalisationData &localisationData, const Ogre::String &id) {
	m_NextMapLocalisationData = localisationData;
	m_NextId = id;
	if (m_NextMapLocalisationData.m_sName == "MAIN_MAP" || m_NextMapLocalisationData.m_sPath == "MAIN_MAP"
      || m_NextMapLocalisationData.m_sName == "MAIN_MAP_NAME" || m_NextMapLocalisationData.m_sPath == "MAIN_MAP_PATH") {
        m_NextMapLocalisationData = MAIN_MAP_LOC_DATA;
	}

	if (m_Fader.getFadeOp() != Fader::FADE_NONE) {return;}
	m_Fader.startFadeOut(0.2);
}
void CMapManager::fadeInCallback() {
	//CHUD::getSingleton().start();
	GameScriptParser::getSingleton().callStartFunction();
}
void CMapManager::fadeOutCallback() {
	Ogre::LogManager::getSingleton().logMessage("Changing map to " + m_NextMapLocalisationData.m_sName);


	if (m_pPlayer) {
		m_pPlayer->destroy();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	if (m_pCurrentMap) {
		m_pCameraController->leaveMap();
		m_pCurrentMap->stop();
	}

	if (m_pCurrentMap != m_pMainMap) {
		delete m_pCurrentMap;
		m_pCurrentMap = NULL;
	}

	if (m_NextMapLocalisationData == MAIN_MAP_LOC_DATA) {
		if (!m_pMainMap) {
			m_pMainMap = new CMainMap(*this, m_pSceneManager, m_NextMapLocalisationData);
			m_pMainMap->load();
		}
		m_pCurrentMap = m_pMainMap;
	}
	else {
		m_pCurrentMap = new CMap(*this, m_pSceneManager, m_NextMapLocalisationData);
		m_pCurrentMap->load();
	}

	auto pCam = m_pSceneManager->getCamera("GameCam");

	if (!m_pPlayer) {
		m_pPlayer = new CPlayer(m_pCurrentMap, m_pCameraController, m_pSceneManager);
		m_pPlayer->init(m_pCurrentMap->getRootSceneNode());
	}

	// parse initialisation script
	GameScriptParser::getSingleton().setCurrentMap(m_pCurrentMap);
	GameScriptParser::getSingleton().parseScript(m_pCurrentMap->getPath() + m_pCurrentMap->getName() + ".lua", m_pCurrentMap->getResourceGroup());
	GameScriptParser::getSingleton().callInitFunction();

	m_pCurrentMap->playerEnters(m_pPlayer, m_NextId);
	m_pCameraController->enterMap(m_pCurrentMap);

	m_pCurrentMap->start();
#if 0
	auto celShadingTexturedVP = Ogre::GpuProgramManager::getSingleton().getByName("Ogre/CelShadingTexturedVP");
	auto celShadingTexturedFP = Ogre::GpuProgramManager::getSingleton().getByName("Ogre/CelShadingTexturedFP");
	auto celShadingVP = Ogre::GpuProgramManager::getSingleton().getByName("Ogre/CelShadingVP");
	auto celShadingFP = Ogre::GpuProgramManager::getSingleton().getByName("Ogre/CelShadingFP");

	auto matIt = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (matIt.hasMoreElements()) {
		Ogre::Material* matNext = static_cast<Ogre::Material*>(matIt.getNext().get());
		if (matNext->getName().find("CS_") != 0) {continue;}
		if (matNext->getNumSupportedTechniques() == 0
			|| matNext->getBestTechnique()->getNumPasses() == 0) {continue;}
		auto pPass = matNext->getBestTechnique()->getPass(0);
		pPass->setIteratePerLight(false);
		if (pPass->hasVertexProgram()) {continue;} // already set
		if (pPass->getNumTextureUnitStates() == 0) {
			pPass->setVertexProgram(celShadingVP->getName());
			pPass->setFragmentProgram(celShadingFP->getName());

			auto params = pPass->getVertexProgramParameters();
			params->setNamedAutoConstant("lightPosition", Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE, 0);
			params->setNamedAutoConstant("eyePosition", Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
			params->setNamedAutoConstant("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			params->setNamedConstant("shininess", 0.3f); // muss float sein!
			params->setNamedAutoConstant("shininess", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);

			params = pPass->getFragmentProgramParameters();
			params->setNamedAutoConstant("diffuse", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
			params->setNamedAutoConstant("specular", Ogre::GpuProgramParameters::ACT_SURFACE_SPECULAR_COLOUR);


			// create 3 new texture states
			Ogre::TextureUnitState *pTUS0 = new Ogre::TextureUnitState(pPass);
			Ogre::TextureUnitState *pTUS1 = new Ogre::TextureUnitState(pPass);
			Ogre::TextureUnitState *pTUS2 = new Ogre::TextureUnitState(pPass);


			pTUS0->setTextureName("cel_shading_diffuse.png", Ogre::TEX_TYPE_1D);
			pTUS0->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS0->setTextureFiltering(Ogre::TFO_NONE);

			pTUS1->setTextureName("cel_shading_specular.png", Ogre::TEX_TYPE_1D);
			pTUS1->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS1->setTextureFiltering(Ogre::TFO_NONE);

			pTUS2->setTextureName("cel_shading_edge.png", Ogre::TEX_TYPE_1D);
			pTUS2->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS2->setTextureFiltering(Ogre::TFO_NONE);

			pPass->addTextureUnitState(pTUS0);
			pPass->addTextureUnitState(pTUS1);
			pPass->addTextureUnitState(pTUS2);
		}
		else {
			pPass->setVertexProgram(celShadingTexturedVP->getName());
			pPass->setFragmentProgram(celShadingTexturedFP->getName());

			auto params = pPass->getVertexProgramParameters();
			params->setNamedAutoConstant("lightPosition", Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE, 0);
			params->setNamedAutoConstant("eyePosition", Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
			params->setNamedAutoConstant("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			params->setNamedConstant("shininess", 0.3f); // muss float sein!


			// create 3 new texture states and move original texture to last state
			Ogre::TextureUnitState *pTUS0 = pPass->getTextureUnitState(0);
			Ogre::TextureUnitState *pTUS1 = new Ogre::TextureUnitState(pPass);
			Ogre::TextureUnitState *pTUS2 = new Ogre::TextureUnitState(pPass);
			Ogre::TextureUnitState *pTUS3 = new Ogre::TextureUnitState(pPass, *pTUS0);

			*pTUS0 = *pTUS1; // reset of TUS0

			pTUS0->setTextureName("cel_shading_diffuse.png", Ogre::TEX_TYPE_1D);
			pTUS0->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS0->setTextureFiltering(Ogre::TFO_NONE);

			pTUS1->setTextureName("cel_shading_specular.png", Ogre::TEX_TYPE_1D);
			pTUS1->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS1->setTextureFiltering(Ogre::TFO_NONE);
			pTUS1->setTextureCoordSet(1);

			pTUS2->setTextureName("cel_shading_edge.png", Ogre::TEX_TYPE_1D);
			pTUS2->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			pTUS2->setTextureFiltering(Ogre::TFO_NONE);
			pTUS2->setTextureCoordSet(2);

			pPass->addTextureUnitState(pTUS1);
			pPass->addTextureUnitState(pTUS2);
			pPass->addTextureUnitState(pTUS3);
		}
	}

#endif

	CHUD::getSingleton().start();
	m_Fader.startFadeIn(0.2);

}
void CMapManager::togglePhysicsDebug() {
	if (m_pCurrentMap) {
		m_pCurrentMap->getPhysicsManager()->toggleDisplayDebugInfo();
	}
}
