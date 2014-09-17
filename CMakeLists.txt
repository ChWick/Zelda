cmake_minimum_required(VERSION 2.8)

# set project name
project(Zelda)
string (TOLOWER ${CMAKE_PROJECT_NAME} LOWERCASE_PROJECT_NAME)
set (GAME_DIR "Zelda")

# set version
set (PROJECT_VERSION_CODE 1)
set (PROJECT_VERSION_NAME 0.0.1)
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set (PROJECT_TEMPLATES_DIR "${CMAKE_MODULE_PATH}/templates")
#include(cotire)

set (PROJECT_STATIC_DEPENDENCIES FALSE)
if (ANDROID)
  message(STATUS "Using static dependencies. Android needs them.")
  set (PROJECT_STATIC_DEPENDENCIES TRUE)
endif()


include(toolchain/AddSdkPaths)

if (APPLE)
	# On apple we use the framework for ogre, ois is included
else()
	# On other systems find Ogre
	find_package(Ogre)
	find_package(boost)
endif()

find_package(bullet)
find_package(OIS)
find_package(CEGUI)
find_package(lua)
#find_package(tinyxml2)

if(ANDROID)
  include(toolchain/AndroidGame)
else()
  add_subdirectory(${GAME_DIR})
endif()

