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

#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/log.h>
#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, "Ogre", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Ogre", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Ogre", __VA_ARGS__))
#else
#include <stdio.h>
#define LOGV(...) printf(__VA_ARGS__); printf("\n")
#define LOGI(...) printf(__VA_ARGS__); printf("\n")
#define LOGW(...) printf(__VA_ARGS__); printf("\n")
#endif

#endif
