#include "StdAfx.h"
#include "Game.h"
#include "DebugDrawer.h"
#include "OgreCodec.h"
#include <OgreOctreeSceneManager.h>
#include <OgreLodListener.h>
#include "GUIManager.h"
#include "MapManager.h"
#include "PlayerData.h"
#include "Player.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "BlinkingMaterialManager.h"
#include "TextLoader.h"
//#include <OgreShaderGenerator.h>
/*#include "DebugDisplay.h"
#include "Dungeon.h"
#include "PCZSceneManager.h"
*/

using namespace Ogre;
class CTestLOD : public Ogre::LodListener {
public:
	virtual bool 	prequeueMovableObjectLodChanged (const MovableObjectLodChangedEvent &evt) {
		//evt.movableObject->setVisible(false);
		return true;
	}

virtual void 	postqueueMovableObjectLodChanged (const MovableObjectLodChangedEvent &evt) {}

virtual bool 	prequeueEntityMeshLodChanged (EntityMeshLodChangedEvent &evt) {

	return true;
}

virtual void 	postqueueEntityMeshLodChanged (const EntityMeshLodChangedEvent &evt) {}

virtual bool 	prequeueEntityMaterialLodChanged (EntityMaterialLodChangedEvent &evt) {
	evt.subEntity->setVisible(evt.newLodIndex < 2);
	/*if (evt.subEntity->getParent()->getUserAny().isEmpty() == false && evt.subEntity->getParent()->getUserAny().getType() == typeid(btRigidBody*)) {
		auto pRB = Ogre::any_cast<btRigidBody*>(evt.subEntity->getParent()->getUserAny());
		if (evt.newLodIndex >= 1) {
			if (CPhysicsManager::getSingleton().getCollisionWorld()->getCollisionObjectArray().findLinearSearch(pRB) != CPhysicsManager::getSingleton().getCollisionWorld()->getCollisionObjectArray().size()) {
				CPhysicsManager::getSingleton().getWorld()->removeRigidBody(pRB);
			}
		}
		else {
			if (CPhysicsManager::getSingleton().getCollisionWorld()->getCollisionObjectArray().findLinearSearch(pRB) == CPhysicsManager::getSingleton().getCollisionWorld()->getCollisionObjectArray().size()) {
				CPhysicsManager::getSingleton().getWorld()->addRigidBody(pRB);
			}
		}
	}*/
	return true;
}

virtual void 	postqueueEntityMaterialLodChanged (const EntityMaterialLodChangedEvent &evt) {}
};
//-------------------------------------------------------------------------------------
CGame::CGame(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
    m_pPlayer = NULL;
}
//-------------------------------------------------------------------------------------
CGame::~CGame(void)
{
    if (mTrayMgr) delete mTrayMgr;

	if (g_bMapDebug) {
		if (mCameraMan) delete mCameraMan;
	}

	if (m_pPlayer) delete m_pPlayer;
    if (DebugDrawer::getSingletonPtr()) delete DebugDrawer::getSingletonPtr();
	//delete CDebugDisplay::getSingletonPtr();
	if (CMapManager::getSingletonPtr()) {delete CMapManager::getSingletonPtr();}
	if (CGUIManager::getSingletonPtr()) delete CGUIManager::getSingletonPtr();
	if (CPlayerData::getSingletonPtr()) delete CPlayerData::getSingletonPtr();

	if (CEGUI::System::getSingletonPtr()) {CEGUI::OgreRenderer::destroySystem();}
	if (CInputListenerManager::getSingletonPtr()) {delete CInputListenerManager::getSingletonPtr();}
	if (CBlinkingMaterialManager::getSingletonPtr()) {delete CBlinkingMaterialManager::getSingletonPtr();}
	if (CTextLoader::getSingletonPtr()) {delete CTextLoader::getSingletonPtr();}

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}
bool CGame::go(void)
{
	new CPlayerData();
	new CBlinkingMaterialManager();
	new CTextLoader();

	// Platform dependent plugins file!
	mPluginsCfg = "plugins";
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	mPluginsCfg += "_windows";
#else
	mPluginsCfg += "_linux";
#endif

#ifdef USE_DEBUG_PLUGINS
    mPluginsCfg += "_d.cfg";
#else
    mPluginsCfg += ".cfg";
#endif

#ifdef USD_DEBUG_RESOURCES
    mResourcesCfg = "resources_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
#endif

    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);
	//mRoot->addSceneManagerFactory(new CPCZSceneManagerFactory());


//-------------------------------------------------------------------------------------
    // setup resources
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
//-------------------------------------------------------------------------------------
    // configure
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "The Legend Of Zelda - A Link To The Past");
    }
    else
    {
        return false;
    }
	CTextLoader::getSingleton().load(CTextLoader::L_ENGLISH);

//-------------------------------------------------------------------------------------
    // choose scenemanager
    // Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE, "MainSceneManager");
	//mSceneMgr = mRoot->createSceneManager("ST_GENERIC");
	//mSceneMgr->addLodListener(new CTestLOD());
	//mSceneMgr->setShadowUseInfiniteFarPlane(false);
//-------------------------------------------------------------------------------------
    // create camera
    // Create the camera
    mCamera = mSceneMgr->createCamera("GameCam");

    // Position it at 500 in Z direction
    //mCamera->setPosition(Ogre::Vector3(0,10,0));
    // Look back along -Z
    //mCamera->lookAt(Ogre::Vector3(10,0,10));
    mCamera->setNearClipDistance(0.01f);
	mCamera->setFarClipDistance(400.0f);

	/*if (g_bMapDebug) {

	}*/
	/*mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
		mCameraMan->setTopSpeed(10);*/
//-------------------------------------------------------------------------------------
    // create viewports
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera, 0);
    //vp->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	vp->setClearEveryFrame(true);
    //vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//-------------------------------------------------------------------------------------
    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
//-------------------------------------------------------------------------------------
    // Create any resource listeners (for loading screens)
    //createResourceListener();
//-------------------------------------------------------------------------------------
    // load resources
	/*Ogre::StringVector extensions = Ogre::Codec::getExtensions();
	for(Ogre::StringVector::iterator itExt = extensions.begin(); itExt != extensions.end(); ++itExt)
	{
		Ogre::StringVectorPtr names = Ogre::ResourceGroupManager::getSingleton().findResourceNames("General",std::string("*.")+*itExt);
		for(Ogre::StringVector::iterator itName = names->begin(); itName!=names->end(); ++itName)
		{
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture","General");
		}
	}*/
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

//-------------------------------------------------------------------------------------
    // Create the scene#
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    // Create a light
    /*Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setPosition(20,80,50);
	l->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
    l->setSpecularColour(Ogre::ColourValue(0.2, 0.2, 0.2));
	l->setDirection(Ogre::Vector3( 0, -2, -1 ));
	l->setCastShadows(false);*/

	//mSceneMgr->setSkyBox(true, "Skybox/Sky");
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::Black,1,30,50);
//-------------------------------------------------------------------------------------
    //create FrameListener
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#else
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	CInputListenerManager *pInputManager = new CInputListenerManager();
    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(pInputManager);
    mKeyboard->setEventCallback(pInputManager);

	pInputManager->addInputListener(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

	//new CDebugDisplay(mTrayMgr);
	new DebugDrawer(mSceneMgr, 0.8f);


	m_pCEGuiOgreRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    //m_pCEGuiImageCodec = &CEGUI::OgreRenderer::createOgreImageCodec();
    //m_pCEGuiResourceProvider = &CEGUI::OgreRenderer::createOgreResourceProvider();*/
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	//new CEGUI::Logger();
	//new CEGUI::WindowManager();
	//new CEGUI::SchemeManager();
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	new CGUIManager();

    mRoot->addFrameListener(this);
//-------------------------------------------------------------------------------------
	Ogre::MeshManager::getSingleton().createPlane(
		"DebugPlane",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO), 1, 1);

    Ogre::LogManager::getSingletonPtr()->logMessage("*** INITIALIZING MapManager ***");
	CMapManager *pMapManager = new CMapManager(mSceneMgr, mSceneMgr->getRootSceneNode());

	pMapManager->init();

    mRoot->startRendering();

    return true;
}

bool CGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown) {
        return false;
	}
	if (!mWindow->isActive()) {
		return true;
	}

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }

		//CDebugDisplay::getSingleton().update();
    }

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	/*if (!CMapManager::getSingleton().getCurrentDungeon()) {
		return true;
	}*/

	CGUIManager::getSingleton().update(evt.timeSinceLastFrame);

	if (!g_bMapDebug) {
		// In normal mode camera is moved by the player and this camera hast so to its position,
		// In map debug mode the game moves the camera and the dungen cam is set to its position
		//mCamera->setPosition(CMapManager::getSingleton().getCurrentDungeon()->getCameraNode()->_getDerivedPosition());
		//mCamera->setOrientation(CMapManager::getSingleton().getCurrentDungeon()->getCameraNode()->_getDerivedOrientation());
	}
	else {
        //
	}

	//mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera


    return true;
}
bool CGame::frameStarted(const Ogre::FrameEvent& evt) {
	if (evt.timeSinceLastFrame > 0.1) {
		return true;
	}
	CMapManager::getSingletonPtr()->update(evt.timeSinceLastFrame);
	CBlinkingMaterialManager::getSingleton().update(evt.timeSinceLastFrame);

	// Right before the frame is rendered, call DebugDrawer::build().
	DebugDrawer::getSingleton().setEnabled(true);
	DebugDrawer::getSingleton().build();
	return true;
}
bool CGame::frameEnded(const Ogre::FrameEvent& evt) {
   // After the frame is rendered, call DebugDrawer::clear()
   DebugDrawer::getSingleton().clear();
   return true;
}
//-------------------------------------------------------------------------------------
bool CGame::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_F2)   // toggle visibility of even rarer debugging details
    {
		/*if (CDebugDisplay::getSingleton().getPanel()->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(CDebugDisplay::getSingleton().getPanel(), OgreBites::TL_TOPLEFT, 0);
            CDebugDisplay::getSingleton().getPanel()->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(CDebugDisplay::getSingleton().getPanel());
            CDebugDisplay::getSingleton().getPanel()->hide();
        }*/
	}
	else if (arg.key == OIS::KC_F5) {
		//CMapManager::getSingleton().reloadDungeon();
	}
	else if (arg.key == OIS::KC_P) {
		CMapManager::getSingleton().togglePhysicsDebug();
	}
    else if (arg.key == OIS::KC_T)   // cycle texture filtering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

		Ogre::SceneManagerEnumerator::SceneManagerIterator iterator = Ogre::Root::getSingleton().getSceneManagerIterator();
		while (iterator.hasMoreElements()) {
			Ogre::SceneManager *pSM = iterator.getNext();
			Ogre::SceneManager::CameraIterator camIt = pSM->getCameraIterator();
			while (camIt.hasMoreElements()) {
				camIt.getNext()->setPolygonMode(pm);
			}
		}
        mDetailsPanel->setParamValue(10, newVal);
    }
	else if (arg.key == OIS::KC_F9) {
		g_bDebugPlayerInteractiveArea = !g_bDebugPlayerInteractiveArea;
	}
	else if (arg.key == OIS::KC_F10) {
		g_bDebugZones = !g_bDebugZones;
	}
	else if (arg.key == OIS::KC_F11) {
		g_bDebugHitboxes = !g_bDebugHitboxes;
	}
	else if (arg.key == OIS::KC_F12) {
		//g_bMapDebug = !g_bMapDebug;
		CMapManager::getSingleton().restartMapLuaScript();
	}
    else if(arg.key == OIS::KC_F6)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(arg.key));
	sys.getDefaultGUIContext().injectChar(arg.text);

	if (g_bMapDebug && mCameraMan) {
		mCameraMan->injectKeyUp(arg);
	}

    return true;
}

bool CGame::keyReleased( const OIS::KeyEvent &arg )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(static_cast<CEGUI::Key::Scan>(arg.key));

	if (g_bMapDebug && mCameraMan) {
		mCameraMan->injectKeyDown(arg);
	}
	return true;
}

bool CGame::mouseMoved( const OIS::MouseEvent &arg )
{
	mTrayMgr->injectMouseMove(arg);
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);

	if (g_bMapDebug && mCameraMan) {
		mCameraMan->injectMouseMove(arg);
	}
	return true;
}
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}
bool CGame::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	mTrayMgr->injectMouseDown(arg, id);
	if (g_bMapDebug && mCameraMan) {
		mCameraMan->injectMouseDown(arg, id);
	}
	return true;
}

bool CGame::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    mTrayMgr->injectMouseUp(arg, id);
	if (g_bMapDebug && mCameraMan) {
		mCameraMan->injectMouseUp(arg, id);
	}
	return true;
}

//Adjust mouse clipping area
void CGame::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void CGame::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

