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

#ifndef _ANDROID_HPP_
#define _ANDROID_HPP_

#ifdef OGRE_STATIC_LIB
#  ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
#    undef OGRE_STATIC_GLES
#    define INCLUDE_RTSHADER_SYSTEM
#    define OGRE_STATIC_GLES2
#  endif
#else
#error "Ogre has to be static!"
#endif

#ifdef INCLUDE_RTSHADER_SYSTEM
#   include "OgreRTShaderSystem.h"
#endif

#  ifdef OGRE_BUILD_PLUGIN_CG
#  define OGRE_STATIC_CgProgramManager
#  endif

#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
#   error This header is for use with Android only
#endif


#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include "OgrePlatform.h"
#include "Android/OgreAndroidEGLWindow.h"
#include "../Config/TypeDefines.hpp"
#include GAME_CLASS_HEADER
#include <stdio.h>

#ifdef OGRE_STATIC_LIB
#  include "OgreStaticPluginLoader.h"
#endif

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Ogre", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Ogre", __VA_ARGS__))

#include "AndroidInput.hpp"
#include "../FileManager/FileManager.hpp"
#include <CEGUI/Exceptions.h>
#include <exception>
#include "../Util/Assert.hpp"

class OgreAndroidBridge;


/*=============================================================================
  | Ogre Android bridge
  =============================================================================*/
class OgreAndroidBridge {
private:
  static bool m_bRenderPaused;
  //static CSnapshot *m_pSnapshot;
public:
  static void init(struct android_app* state) {
    mActivity = state->activity;
    state->onAppCmd = &OgreAndroidBridge::handleCmd;
    state->onInputEvent = &OgreAndroidBridge::handleInput;
  }
  static void start() {
    ASSERT(mActivity);

    if(mInit)
      return;

    LOGI("Initialising Root");
    mRoot = new Ogre::Root("plugins"OGRE_BUILD_SUFFIX".cfg",
			   "ogre.cfg",
			   CFileManager::getValidPath("ogre.log",
						      CFileManager::SL_EXTERNAL));
#ifdef OGRE_STATIC_LIB
    LOGI("Loading plugins");
    mStaticPluginLoader = new Ogre::StaticPluginLoader();
    mStaticPluginLoader->load();
    LOGI("plugins loaded");
#endif
    mRoot->setRenderSystem(mRoot->getAvailableRenderers().at(0));
    mRoot->initialise(false);
    mInit = true;
  }

  static void shutdown()
  {
    if(!mInit)
      return;

    mInit = false;

    if(mGame)
      {
	mGame->closeApp();
	OGRE_DELETE mGame;
	mGame = NULL;
      }

    OGRE_DELETE mRoot;
    mRoot = NULL;
    mRenderWnd = NULL;

    delete mTouch;
    mTouch = NULL;

    delete mKeyboard;
    mKeyboard = NULL;

    delete mInputInjector;
    mInputInjector = NULL;

#ifdef OGRE_STATIC_LIB
    mStaticPluginLoader->unload();
    delete mStaticPluginLoader;
    mStaticPluginLoader = NULL;
#endif
  }

  static int32_t handleInput(struct android_app* app, AInputEvent* event)
  {
    if (mInputInjector)
      {
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	  {
	    mTouch->setStatesToNone(AMotionEvent_getPointerCount(event));
	    for (size_t n = 0; n < AMotionEvent_getPointerCount(event); n++) {
	      int nPointerId = AMotionEvent_getPointerId( event, n );
	      int nAction = AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event );
	      int nRawAction = AMotionEvent_getAction( event );
	      int nPointerIndex = n;

	      if( nAction == AMOTION_EVENT_ACTION_POINTER_DOWN || nAction == AMOTION_EVENT_ACTION_POINTER_UP)
		{
		  nPointerIndex = (AMotionEvent_getAction( event ) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		  nPointerId = AMotionEvent_getPointerId( event, nPointerIndex );
		}

	      int action = nAction;
	      int id = nPointerId;
	      if(action == 0 || action == 5)
		mInputInjector->
		  injectTouchEvent(2,
				   AMotionEvent_getRawX(event, nPointerIndex),
				   AMotionEvent_getRawY(event, nPointerIndex), nPointerIndex );

	      mInputInjector->
		injectTouchEvent(action,
				 AMotionEvent_getRawX(event, nPointerIndex),
				 AMotionEvent_getRawY(event, nPointerIndex), nPointerIndex );
	      //LOGI("Action of %d: %d", pi, action);
	      if (nAction != 2) {
		// not moved action, do not iterate
		break;
	      }
	    }
	    //LOGI("Pointer Count: %d  Number of states: %d", AMotionEvent_getPointerCount(event), mTouch->getMultiTouchStates().size());
	  }
	else
	  {
	    mInputInjector->injectKeyEvent(AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event));
	  }

	return 1;
      }
    return 0;
  }

  static void handleCmd(struct android_app* app, int32_t cmd)
  {
    if (app->savedState) {
      LOGI("loading snapshot ...");
      //CSnapshotManager::getSingleton().setSnapshot(std::shared_ptr<CSnapshot>(new CSnapshot(app->savedState, app->savedStateSize)));
    }
    switch (cmd)
      {
      case APP_CMD_SAVE_STATE:
	LOGI("Saving state");
	//CSnapshotManager::getSingleton().makeBackupSnapshot();
	//CSnapshotManager::getSingleton().makeSnapshot().saveToMemory(app->savedState, app->savedStateSize);
	//if (CSaveStateManager::getSingletonPtr()) {CSaveStateManager::getSingleton().writeXMLFile();}
	break;
      case APP_CMD_INIT_WINDOW:
	m_bRenderPaused = false;
	LOGI("Initialising window command");
	if (app->window && mRoot) {
	  LOGI("... creating config");
	  AConfiguration* config = AConfiguration_new();
	  AConfiguration_fromAssetManager(config, app->activity->assetManager);

	  if (!mRenderWnd) {
	    LOGI("... creating render window");
	    Ogre::NameValuePairList opt;
	    opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)app->window);
	    opt["androidConfig"] = Ogre::StringConverter::toString((int)config);

	    mRenderWnd = Ogre::Root::getSingleton().createRenderWindow("OgreWindow", 0, 0, false, &opt);

	    if(!mTouch)
	      mTouch = new AndroidMultiTouch();

	    if(!mKeyboard)
	      mKeyboard = new AndroidKeyboard();

	    if(!mGame) {
	      LOGI("... creating game");
	      mGame = OGRE_NEW GAME_CLASS();
	      mGame->initAppForAndroid(mRenderWnd, app, mTouch, mKeyboard);
	      mGame->initApp();

	      mInputInjector = new AndroidInputInjector(CInputListenerManager::getSingletonPtr(), mTouch, mKeyboard);
	    }
	  }
	  else {
	    LOGI("... recreating render winow");
	    static_cast<Ogre::AndroidEGLWindow*>(mRenderWnd)->_createInternalResources(app->window, config);
	    if (mGame) {
	      LOGI("... creating resources");
	      mGame->createResources();
	    }
	  }

	  AConfiguration_delete(config);
	}
	break;
      case APP_CMD_TERM_WINDOW:
	if (mGame)
	  mGame->destroyResources();

	if(mRoot && mRenderWnd)
	  static_cast<Ogre::AndroidEGLWindow*>(mRenderWnd)->_destroyInternalResources();
	break;
      case APP_CMD_PAUSE:
	m_bRenderPaused = true;
	break;
      case APP_CMD_RESUME:
	m_bRenderPaused = false;
	break;
      case APP_CMD_GAINED_FOCUS:
	break;
      case APP_CMD_LOST_FOCUS:
	break;
      case APP_CMD_CONFIG_CHANGED:
	break;
      }
  }

  static bool renderOneFrame(struct android_app* state) {
    int ident, events;
    struct android_poll_source* source;

    while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
      {
	try {
	  if (source != NULL)
	    source->process(state, source);

	  if (state->destroyRequested != 0)
	    return false;
	}
	catch (const Ogre::Exception &e) {
	  LOGW("Error in rendering loop");
	  LOGW("%s", e.getFullDescription().c_str());
	}
	catch (const CEGUI::Exception &e) {
	  LOGW("Exception: %s", e.getMessage().c_str());
	}
	catch (const std::exception &e) {
	  LOGW("Exception: %s", e.what());
	}
	catch (...) {
	  LOGW("Unknown Exception in %s", __FILE__);
	}
      }

    if(mRenderWnd != NULL && mRenderWnd->isActive() && !m_bRenderPaused)
      {
	try {
	  mRenderWnd->windowMovedOrResized();
	  if (!mRoot->renderOneFrame()) {
	    ANativeActivity_finish(state->activity);
	  }
	}
	catch (const Ogre::Exception &e) {
	  LOGW("Error in rendering loop");
	  LOGW("%s", e.getFullDescription().c_str());
	  ANativeActivity_finish(state->activity);
	}
	catch (const CEGUI::Exception &e) {
	  LOGW("Exception: %s", e.getMessage().c_str());
	  ANativeActivity_finish(state->activity);
	}
	catch (const std::exception &e) {
	  LOGW("Exception: %s", e.what());
	}
	catch (...) {
	  LOGW("Unknown Exception in %s", __FILE__);
	  ANativeActivity_finish(state->activity);
	}
      }
    return true;
  }

  static void go(struct android_app* state)
  {
    if (state->savedState) {
      LOGI("loading snapshot ...");
      //CSnapshotManager::getSingleton().setSnapshot(std::shared_ptr<CSnapshot>(new CSnapshot(state->savedState, state->savedStateSize)));
    }
    else {
      LOGI("no snapshot found, not loading");
    }
    LOGI("starting rendering");
    while (renderOneFrame(state)) {}
  }

  static Ogre::RenderWindow* getRenderWindow()
  {
    return mRenderWnd;
  }

  static void showAdPopup() {
    callJavaVoid("showAdPopup");
  }
  static void closeAd() {
    callJavaVoid("closeAd");
  }
  static bool adPopupClosed() {
    // Get the android application's activity.
    ANativeActivity* activity = mActivity;
    JavaVM* jvm = mActivity->vm;
    JNIEnv* env = NULL;
    (jvm)->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = (jvm)->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR) {
      LOGI("Failed to retrieve JVM environment");
      // Failed to retrieve JVM environment
      return true;
    }
    jclass clazz = (env)->GetObjectClass(activity->clazz);
    jmethodID methodID = (env)->GetMethodID(clazz, "adPopupClosed", "()Z");
    if (!methodID) {
      LOGW("Method adPopupClosed not found");
    }
    jboolean r = (env)->CallBooleanMethod(activity->clazz, methodID);
    if(env->ExceptionOccurred() != NULL) {
      LOGW("Exception while calling adPopupClosed().");
    }
    (jvm)->DetachCurrentThread();

    return r == JNI_TRUE;
  }
  static void callJavaVoid(const char *func) {
    // Get the android application's activity.
    ANativeActivity* activity = mActivity;
    JavaVM* jvm = mActivity->vm;
    JNIEnv* env = NULL;
    (jvm)->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = (jvm)->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR) {
      LOGI("Failed to retrieve JVM environment");
      // Failed to retrieve JVM environment
      return;
    }
    jclass clazz = (env)->GetObjectClass(activity->clazz);
    jmethodID methodID = (env)->GetMethodID(clazz, func, "()V");
    (env)->CallVoidMethod(activity->clazz, methodID);
    (jvm)->DetachCurrentThread();
  }

  static void callJavaVoid(const char *func, const std::string &str) {
    // Get the android application's activity.
    ANativeActivity* activity = mActivity;
    JavaVM* jvm = mActivity->vm;
    JNIEnv* env = NULL;
    (jvm)->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = (jvm)->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR) {
      LOGI("Failed to retrieve JVM environment");
      // Failed to retrieve JVM environment
      return;
    }
    jclass clazz = (env)->GetObjectClass(activity->clazz);
    jmethodID methodID = (env)->GetMethodID(clazz, func, "(Ljava/lang/String;)V");
    if(!methodID) {
      LOGI("method %s does not exist", func);
      return;
    }
    jstring jString = (env)->NewStringUTF( str.c_str() );
    (env)->CallVoidMethod(activity->clazz, methodID, jString);
    (jvm)->DetachCurrentThread();
  }

  static std::string callJavaString(const char *func) {
    // Get the android application's activity.
    ANativeActivity* activity = mActivity;
    JavaVM* jvm = mActivity->vm;
    JNIEnv* env = NULL;
    (jvm)->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = (jvm)->AttachCurrentThread(&env, NULL);
    if (res == JNI_ERR) {
      LOGI("Failed to retrieve JVM environment");
      // Failed to retrieve JVM environment
      return "";
    }
    jclass clazz = (env)->GetObjectClass(activity->clazz);
    jmethodID methodID = (env)->GetMethodID(clazz, func, "()Ljava/lang/String;");
    if(!methodID) {
      LOGI("method %s does not exist", func);
      return "";
    }
    jstring jString = (jstring)(env)->CallObjectMethod(activity->clazz, methodID);
    std::string s =  env->GetStringUTFChars(jString, JNI_FALSE);
    (jvm)->DetachCurrentThread();
    return s;
  }

private:
  static ANativeActivity *mActivity;
  static CGame* mGame;
  static AndroidInputInjector* mInputInjector;
  static AndroidMultiTouch* mTouch;
  static AndroidKeyboard* mKeyboard;
  static Ogre::RenderWindow* mRenderWnd;
  static Ogre::Root* mRoot;
  static bool mInit;

#ifdef OGRE_STATIC_LIB
  static Ogre::StaticPluginLoader* mStaticPluginLoader;
#endif
};

#endif
