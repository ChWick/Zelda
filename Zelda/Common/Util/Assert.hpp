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

#ifndef _ASSERT_HPP_
#define _ASSERT_HPP_

#include <assert.h>
#include "GlobalBuildDefines.hpp"
#include <OgrePlatform.h>

#ifndef PROJECT_ASSERT_MODE
#error "Project assert mode is undefined. Try to rerun cmake build script."

#elif PROJECT_ASSERT_MODE == 0
#define ASSERT(cond)
#elif PROJECT_ASSERT_MODE == 1

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
//#define ASSERT(e...) __android_log_assert(e, "TAG", #e)
//#define ASSERT(e) assert(e)
#define ASSERT(e) ((e) ? (void)0 : __android_log_assert(0,PROJECT_NAME,"%s(%s:%d) >> %s ",__func__ ,__FILE__, __LINE__, #e))
#else

#define ASSERT(cond) assert(cond)

#endif

#endif // _PROJECT_ASSERT_MODE

#endif // _ASSERT_HPP_
