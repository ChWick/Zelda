################################################################################
# Custom cmake module for zelda to find PAGED_GEOMETRY
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PAGED_GEOMETRY_H_PATH NAMES PagedGeometry.h PATH_SUFFIXES PagedGeometry)
find_library(PAGED_GEOMETRY_LIB NAMES PagedGeometry)
find_library(PAGED_GEOMETRY_LIB_DBG NAMES PagedGeometry_d)
mark_as_advanced(PAGED_GEOMETRY_H_PATH PAGED_GEOMETRY_LIB PAGED_GEOMETRY_LIB_DBG)

find_package_handle_standard_args(PAGED_GEOMETRY DEFAULT_MSG PAGED_GEOMETRY_LIB PAGED_GEOMETRY_H_PATH)


# set up output vars
if (PAGED_GEOMETRY_FOUND)
    set (PAGED_GEOMETRY_INCLUDE_DIR ${PAGED_GEOMETRY_H_PATH})
    set (PAGED_GEOMETRY_LIBRARIES ${PAGED_GEOMETRY_LIB})
    if (PAGED_GEOMETRY_LIB_DBG)
        set (PAGED_GEOMETRY_LIBRARIES_DBG ${PAGED_GEOMETRY_LIB_DBG})

    endif()
else()
    set (PAGED_GEOMETRY_OCTREE_INCLUDE_DIR)
    set (PAGED_GEOMETRY_INCLUDE_DIR)
    set (PAGED_GEOMETRY_LIBRARIES)
    set (PAGED_GEOMETRY_LIBRARIES_DBG)
endif()


