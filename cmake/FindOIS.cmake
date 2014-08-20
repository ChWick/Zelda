################################################################################
# Custom cmake module for CEGUI to find OIS
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OIS_H_PATH NAMES OIS.h PATH_SUFFIXES OIS PATHS ${OGRE_DEPS_ROOT}/include)
find_library(OIS_LIB NAMES OIS libOIS PATHS ${OGRE_DEPS_ROOT}/lib)
find_library(OIS_LIB_DBG NAMES OIS_d libOIS_d PATHS ${OGRE_DEPS_ROOT}/lib)

mark_as_advanced(OIS_H_PATH OIS_LIB OIS_LIB_DBG)

find_package_handle_standard_args(OIS DEFAULT_MSG OIS_LIB OIS_H_PATH)


# set up output vars
if (OIS_FOUND)
    set (OIS_INCLUDE_DIR ${OIS_H_PATH})
    set (OIS_LIBRARIES ${OIS_LIB})
    if (OIS_LIB_DBG)
        set (OIS_LIBRARIES_DBG ${OIS_LIB_DBG})
    endif()
else()
    set (OIS_INCLUDE_DIR)
    set (OIS_LIBRARIES)
    set (OIS_LIBRARIES_DBG)
endif()


