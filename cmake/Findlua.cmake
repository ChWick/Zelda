################################################################################
# Custom cmake module for CMAKE to find LUA
################################################################################
include(FindPackageHandleStandardArgs)

find_path(LUA_H_PATH NAMES lua.hpp)
find_library(LUA_LIB NAMES lua)
find_library(LUA_LIB_DBG NAMES liblua5.2_d liblua_d)

mark_as_advanced(LUA_H_PATH LUA_LIB LUA_LIB_DBG)

find_package_handle_standard_args(LUA DEFAULT_MSG LUA_LIB LUA_H_PATH)


# set up output vars
if (LUA_FOUND)
    set (LUA_INCLUDE_DIR ${LUA_H_PATH})
    set (LUA_LIBRARIES ${LUA_LIB})
    if (LUA_LIB_DBG)
        set (LUA_LIBRARIES_DBG ${LUA_LIB_DBG})
    endif()
else()
    set (LUA_INCLUDE_DIR)
    set (LUA_LIBRARIES)
    set (LUA_LIBRARIES_DBG)
endif()


