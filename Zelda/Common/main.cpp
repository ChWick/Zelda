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

#include <CEGUI/CEGUI.h>
#include "Config/TypeDefines.hpp"
#include GAME_CLASS_HEADER
#include <OIS.h>
#include "FileManager/FileManager.hpp"
#include "XMLResources/Manager.hpp"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_DEFAULT_LIBS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#pragma comment(lib,"user32.lib")
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "OSX/OSX.hpp"
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#include "SampleBrowser_iOS.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include "Android/Android.hpp"
#include <jni.h>

CGame* OgreAndroidBridge::mGame = NULL;
AndroidInputInjector* OgreAndroidBridge::mInputInjector = NULL;
AndroidMultiTouch* OgreAndroidBridge::mTouch = NULL;
AndroidKeyboard* OgreAndroidBridge::mKeyboard = NULL;
Ogre::RenderWindow* OgreAndroidBridge::mRenderWnd = NULL;
Ogre::Root* OgreAndroidBridge::mRoot = NULL;
bool OgreAndroidBridge::mInit = false;
bool OgreAndroidBridge::m_bRenderPaused = true;
//CSnapshot *OgreAndroidBridge::m_pSnapshot = NULL;
ANativeActivity *OgreAndroidBridge::mActivity = NULL;

#   ifdef OGRE_STATIC_LIB
Ogre::StaticPluginLoader* OgreAndroidBridge::mStaticPluginLoader = NULL;
#   endif

#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
void android_main(struct android_app* state)
#else
int main(int argc, char *argv[])
#endif
{
    
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
	[pool release];
	return retVal;
#elif (OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    mAppDelegate = [[AppDelegate alloc] init];
    [[NSApplication sharedApplication] setDelegate:mAppDelegate];
	int retVal = NSApplicationMain(argc, (const char **) argv);
    
	[pool release];
    
	return retVal;
#elif OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  OgreAndroidBridge::init(state);


  LOGI("Starting");
  app_dummy();

  LOGI("Init FileManager");
  CFileManager::init(state->activity);


  try {
    XMLResources::CManager::LANGUAGE_CODE = OgreAndroidBridge::callJavaString("getLanguage");
    LOGI("loading language: %s", XMLResources::CManager::LANGUAGE_CODE.c_str());
    XMLResources::GLOBAL.loadLanguage();
  }
  catch (const Ogre::Exception& e) {
    LOGW("An exception has occured: %s", e.getFullDescription().c_str());
    // dont quit, use default language
  }

  LOGI("Init Settings");
  //new CSettings();

  LOGI("Initialize");
  OgreAndroidBridge::start();
  OgreAndroidBridge::callJavaVoid("setLoadText", "Go");
  LOGI("Go");
  OgreAndroidBridge::go(state);
  LOGI("End");
#else
  CFileManager::init();
  try {
    XMLResources::CManager::LANGUAGE_CODE = "de";
    XMLResources::GLOBAL.loadLanguage();
  }
  catch (const Ogre::Exception& e) {
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#  else
    std::cerr << "An exception has occured: " <<
      e.getFullDescription().c_str() << std::endl;
    std::cout << "An exception hat occured: " <<
      e.getFullDescription().c_str() << std::endl;
#  endif
  }

  // Create application object
  GAME_CLASS *app = new GAME_CLASS();

#    if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    else
  if (argc == 2) {
    // add additional level dir path
    app->getAdditionalLevelDirPaths().push_back(argv[1]);
  }
#    endif

  try {
    app->go();
  } catch( Ogre::Exception& e ) {
#    if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#    else
    std::cerr << "An exception has occured: " <<
      e.getFullDescription().c_str() << std::endl;
    std::cout << "An exception hat occured: " <<
      e.getFullDescription().c_str() << std::endl;
#    endif
  }
  catch (CEGUI::Exception &e) {
#    if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getMessage().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#    else
    std::cerr << "An exception has occured: " <<
      e.getMessage().c_str() << std::endl;
#    endif
  }
  catch (...) {
#    if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, "Unknown Error", "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#    else
    std::cerr << "An exception has occured: " <<
      "Unknown Error" << std::endl << "at file: " << __FILE__ << std::endl;
#    endif
  }
  
  delete app;
#  endif

#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
  return 0;
#endif
}
