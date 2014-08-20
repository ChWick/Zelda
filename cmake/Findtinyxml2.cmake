################################################################################
# Custom cmake module for CEGUI to find tinyxml2
################################################################################
include(FindPackageHandleStandardArgs)

find_path(TINYXML2_H_PATH NAMES tinyxml2.h PATH_SUFFIXES tinyxml PATHS ${TINYXML2_SDK_ROOT}/include)
find_library(TINYXML2_LIB NAMES tinyxml2 libtinyxml2 PATHS ${TINYXML2_SDK_ROOT}/lib)
find_library(TINYXML2_LIB_DBG NAMES tinyxml2_d libtinyxml2_d PATHS ${TINYXML2_SDK_ROOT}/lib)

mark_as_advanced(TINYXML2_H_PATH TINYXML2_LIB TINYXML2_LIB_DBG)

find_package_handle_standard_args(TINYXML2 DEFAULT_MSG TINYXML2_LIB TINYXML2_H_PATH)


# set up output vars
if (TINYXML2_FOUND)
    set (TINYXML2_INCLUDE_DIR ${TINYXML2_H_PATH})
    set (TINYXML2_LIBRARIES ${TINYXML2_LIB})
    if (OIS_LIB_DBG)
        set (TINYXML2_LIBRARIES_DBG ${TINYXML2_LIB_DBG})
    endif()
else()
    set (TINYXML2_INCLUDE_DIR)
    set (TINYXML2_LIBRARIES)
    set (TINYXML2_LIBRARIES_DBG)
endif()


