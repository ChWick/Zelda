################################################################################
# Custom cmake module for zelda to find BULLET
################################################################################
include(FindPackageHandleStandardArgs)

find_path(BULLET_H_PATH NAMES btBulletCollisionCommon.h PATH_SUFFIXES bullet)
find_library(BULLET_LINEAR_MATH_LIB NAMES LinearMath)
find_library(BULLET_DYNAMICS_LIB NAMES BulletDynamics)
find_library(BULLET_COLLISION_LIB NAMES BulletCollision)
find_library(BULLET_SOFT_BODY_LIB NAMES BulletSoftBody)
#find_library(BULLET_CONVEX_DECOMPOSITION_LIB NAMES ConvexDecomposition)
find_library(BULLET_LIB_DBG NAMES BulletCollision_d LinearMath_d BulletDynamics_d ConvexDecomposition)
mark_as_advanced(BULLET_H_PATH BULLET_LINEAR_MATH_LIB BULLET_DYNAMICS_LIB BULLET_COLLISION_LIB BULLET_CONVEX_DECOMPOSITION_LIB BULLET_LIB_DBG)

find_package_handle_standard_args(BULLET DEFAULT_MSG BULLET_LINEAR_MATH_LIB  BULLET_DYNAMICS_LIB BULLET_COLLISION_LIB BULLET_SOFT_BODY_LIB BULLET_H_PATH)
#find_package_handle_standard_args(BULLET DEFAULT_MSG BULLET_CONVEX_DECOMPOSITION_LIB BULLET_H_PATH)


# set up output vars
if (BULLET_FOUND)
    set (BULLET_INCLUDE_DIR ${BULLET_H_PATH})
    set (BULLET_LIBRARIES ${BULLET_SOFT_BODY_LIB} ${BULLET_DYNAMICS_LIB} ${BULLET_COLLISION_LIB} ${BULLET_LINEAR_MATH_LIB} ${BULLET_CONVEX_DECONPOSITION} )
    if (BULLET_LIB_DBG)
        set (BULLET_LIBRARIES_DBG ${BULLET_LIB_DBG})

    endif()
else()
    set (BULLET_OCTREE_INCLUDE_DIR)
    set (BULLET_INCLUDE_DIR)
    set (BULLET_LIBRARIES)
    set (BULLET_LIBRARIES_DBG)
endif()


