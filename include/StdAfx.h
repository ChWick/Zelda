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

#include "libraries/OgreSdkUtil/SdkCameraMan.h"
#include "libraries/OgreSdkUtil/SdkTrays.h"

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

#include "PagedGeometryConfig.h"
#include "PagedGeometry.h"
#include "BatchPage.h"
#include "ImpostorPage.h"
#include "TreeLoader3D.h"

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
