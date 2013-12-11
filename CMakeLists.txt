cmake_minimum_required(VERSION 2.8)

project(zelda)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cotire)

find_package(Ogre)
find_package(OIS)
find_package(CEGUI)
find_package(bullet)
find_package(PagedGeometry)
find_package(lua)

#add_subdirectory(include)
add_subdirectory(src)

#install(TARGETS zelda RUNTIME DESTINATION bin)

