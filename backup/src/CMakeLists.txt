#project(zelda)

file(GLOB zelda_source_files *.cpp)
file(GLOB zelda_include_files *.h)

ADD_DEFINITIONS(-std=c++11)

set(zelda_include_directories
    ..
    ../include
    ../libraries/OgreSdkUtil/
    ${OIS_INCLUDE_DIR}
    ${OGRE_INCLUDE_DIR}
    ${OGRE_OCTREE_INCLUDE_DIR}
    ${CEGUI_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIR}
    ${PAGED_GEOMETRY_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
)
set(zelda_link_libraries
    boost_system
    boost_thread
    boost_regex
    pthread
    ${OIS_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${CEGUI_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${PAGED_GEOMETRY_LIBRARIES}
    ${LUA_LIBRARIES}
)
set(EXECUTABLE_OUTPUT_PATH ../bin)
include_directories(${zelda_include_directories})
add_executable(zelda ${zelda_source_files} ${zelda_include_files})
target_link_libraries(zelda ${zelda_link_libraries})
set_target_properties(zelda PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "StdAfx.cpp")
cotire(zelda)
