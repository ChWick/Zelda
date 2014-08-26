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

#ifndef _STD_AFX_H_
#define _STD_AFX_H_

#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <Ogre.h>
#include "OgreResource.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"
#include "OgreOverlayManager.h"
#include <OgreOctreeSceneManager.h>
#include "OgreSingleton.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <OIS.h>
#else
#include <OIS/OIS.h>
#endif

#include "dependencies/OgreSdkUtil/SdkCameraMan.h"
#include "dependencies/OgreSdkUtil/SdkTrays.h"

#include <CEGUI/CEGUI.h>

#include "tinyxml2.h"

//#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

#include <lua.hpp>

/*#include "Hydrax/Hydrax.h"
#include "Hydrax/Noise/Perlin/Perlin.h"
#include "Hydrax/Modules/ProjectedGrid/ProjectedGrid.h"
#include "Hydrax/Modules/SimpleGrid/SimpleGrid.h"*/

#include <CEGUI/RendererModules/Ogre/ImageCodec.h>
#include <CEGUI/RendererModules/Ogre/ResourceProvider.h>

#ifdef _WIN32
// #include <vld.h>
#endif

#ifdef _DEBUG
//#define USE_DEBUG_PLUGINS
//#define USE_DEBUG_RESOURCES
#endif

#endif
