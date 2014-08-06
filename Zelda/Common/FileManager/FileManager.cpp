/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "FileManager.hpp"
#include "OgreLogManager.h"
#include <iostream>
#include <string>
#include <sys/stat.h>


#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

typedef struct stat Stat;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <direct.h>
#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

typedef int mode_t;
int mkdir(const char *path, mode_t mode) {
  return _mkdir(path);
}
#endif

#ifndef lint
/* Prevent over-aggressive optimizers from eliminating ID string */
const char jlss_id_mkpath_c[] = "@(#)$Id: mkpath.c,v 1.13 2012/07/15 00:40:37 jleffler Exp $";
#endif /* lint */

static int do_mkdir(const char *path, mode_t mode)
{
    Stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(path, mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }

    return(status);
}

/**
** mkpath - ensure all directories in path exist
** Algorithm takes the pessimistic view and works top-down to ensure
** each directory in path exists, rather than optimistically creating
** the last element and working backwards.
*/
int mkpath(const char *path, mode_t mode)
{
    char           *pp;
    char           *sp;
    int             status;
    char           *copypath = strdup(path);

    status = 0;
    pp = copypath;
    while (status == 0 && (sp = strchr(pp, '/')) != 0)
    {
        if (sp != pp)
        {
	  /* Neither root nor double slash in path */
            *sp = '\0';
            status = do_mkdir(copypath, mode);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == 0)
        status = do_mkdir(path, mode);
    free(copypath);
    return (status);
}

// define paths
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
const std::string CFileManager::DIRECTORY_LEVEL("level/");
#else
const std::string CFileManager::DIRECTORY_LEVEL("../level/user/");
#endif

bool CFileManager::m_bInitialized(false);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
ANativeActivity *CFileManager::mNativeActivity(NULL);
#endif


std::string CFileManager::getValidPath(const std::string &sFileName,
					 EStorageLocation eLocation) {
  assert(m_bInitialized);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  ANativeActivity* nativeActivity = mNativeActivity;
  std::string dataPath;
  if (eLocation == SL_INTERNAL) {
    dataPath = nativeActivity->internalDataPath;
  }
  else {
    dataPath = nativeActivity->externalDataPath;
  }

  std::string addPath(sFileName.substr(0, sFileName.rfind("/")));
  std::string path = dataPath;
  if (addPath.size() > 0 && addPath.size() != sFileName.size()) {
    // there is a additional directory(s) in from
    path += "/" + addPath;
  }

  struct stat sb;
  int32_t res = stat(path.c_str(), &sb);
  if (0 == res && sb.st_mode & S_IFDIR) {
    // path already exists
  }
  else {
    // create path
    res = mkpath(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (res != 0) {
      // error...
    }
  }

  return dataPath + "/" + sFileName;
#else
  return sFileName;
#endif
}
bool CFileManager::openFile(std::fstream &stream,
			    const std::string &sFileName,
			    std::ios_base::openmode mode,
			    EStorageLocation eLocation) {
  assert(m_bInitialized);
  std::string path(getValidPath(sFileName, eLocation));
  stream.open(path, mode);
  if (!stream) {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "Creating file at path: " + path);
    if (FILE *f = fopen(path.c_str(), "w+")) {
      fclose(f);
      stream.open(path, mode);
    }
    if (!stream) {
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "File " + path + " not found");
      return false;
    }
  }
  if (stream) {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "File " + path  + " openened");
  }
  return true;
}

void CFileManager::deleteFile(const std::string &sFileName,
			      EStorageLocation eLocation) {
  if (remove(getValidPath(sFileName, eLocation).c_str()) != 0) {
    Ogre::LogManager::getSingleton().logMessage("File " + sFileName + " could not be deleted");
  }
  else {
    Ogre::LogManager::getSingleton().logMessage("File " + sFileName + " has been deleted");
  }
}

Ogre::DataStreamPtr CFileManager::openDataStream(const std::string& fileName) {
  Ogre::DataStreamPtr stream;
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
  AAsset* asset = AAssetManager_open(mNativeActivity->assetManager, fileName.c_str(), AASSET_MODE_BUFFER);
  if(asset) {
    off_t length = AAsset_getLength(asset);
    void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
    memcpy(membuf, AAsset_getBuffer(asset), length);
    AAsset_close(asset);

    stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
  }
#else
  // first one gives a memory leak
  //stream = Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(new std::fstream(fileName)));
  // second one results in strange crash (segmentation fault on closing)
  //stream = Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(OGRE_NEW_T( std::fstream, Ogre::MEMCATEGORY_GENERAL )( fileName.c_str(), std::fstream::in ) ));
  // this one works
  FILE *pFile = fopen(fileName.c_str(), "r");
  if (!pFile) {
    return Ogre::DataStreamPtr();
  }
  stream = Ogre::DataStreamPtr(new Ogre::FileHandleDataStream(pFile));
#endif
  return stream;
}
