#ifndef _GAME_H_
#define _GAME_H_

#include "StdAfx.h"
#include "InputListener.h"

class CPlayer;
class CPhysicsManager;

class CGame : public CInputListener, public Ogre::FrameListener, public Ogre::WindowEventListener, OgreBites::SdkTrayListener

{
private:
    CPlayer *m_pPlayer;

	CEGUI::Renderer* m_pCEGuiOgreRenderer;
    //! ImageCodec to use.  Set in subclass constructor, may be 0.
    CEGUI::ImageCodec* m_pCEGuiImageCodec;
    //! ResourceProvider to use.  Set in subclass constructor, may be 0.
    CEGUI::ResourceProvider* m_pCEGuiResourceProvider;
public:
    CGame(void);
    virtual ~CGame(void);
    bool go(void);

	OIS::Mouse*    getMouse() {return mMouse;}
	OIS::Keyboard* getKeyboard() {return mKeyboard;}
protected:
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;      // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;    // sample details panel
    bool mCursorWasVisible;                   // was cursor visible before dialog appeared
    bool mShutDown;

    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif // #ifndef _GAME_H_
