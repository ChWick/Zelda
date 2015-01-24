################################################################################
# Custom cmake module to find ParticleUniverse
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PARTICLEUNIVERSE_H_PATH NAMES ParticleUniverseSystemManager.h PATH_SUFFIXES ParticleUniverse PATHS)
find_library(PARTICLEUNIVERSE_LIB NAMES Plugin_ParticleUniverse.so Plugin_ParticleUniverse.lib PATH_SUFFIXES ParticleUniverse)
find_library(PARTICLEUNIVERSE_LIB_DBG NAMES Plugin_ParticleUniverse_d PATH_SUFFIXES ParticleUniverse)

mark_as_advanced(PARTICLEUNIVERSE_H_PATH PARTICLEUNIVERSE_LIB PARTICLEUNIVERSE_LIB_DBG)

find_package_handle_standard_args(PARTICLEUNIVERSE DEFAULT_MSG PARTICLEUNIVERSE_LIB PARTICLEUNIVERSE_H_PATH)


# set up output vars
if (PARTICLEUNIVERSE_FOUND)
    set (PARTICLEUNIVERSE_INCLUDE_DIR ${PARTICLEUNIVERSE_H_PATH})
    set (PARTICLEUNIVERSE_LIBRARIES ${PARTICLEUNIVERSE_LIB})
    if (PARTICLEUNIVERSE_LIB_DBG)
        set (PARTICLEUNIVERSE_LIBRARIES ${PARTICLEUNIVERSE_LIB_DBG})
    endif()
else()
    set (PARTICLEUNIVERSE_INCLUDE_DIR)
    set (PARTICLEUNIVERSE_LIBRARIES)
    set (PARTICLEUNIVERSE_LIBRARIES_DBG)
endif()
