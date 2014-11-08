project(Game)


SET(SOURCE_DIR "@CMAKE_SOURCE_DIR@/${GAME_DIR}")

file(GLOB_RECURSE Game_source_files "@SOURCE_DIR@/*.cpp")
#file(GLOB Game_source_event_files Events/*.cpp)
#file(GLOB Game_source_xmlresources_files XMLResources/*.cpp)
#file(GLOB Game_source_xmlresources_files XMLResources/*.cpp)
file(GLOB_RECURSE Game_include_files "@SOURCE_DIR@/*.hpp")

ADD_DEFINITIONS(-std=c++11 -Wall)


# create global build defines
set (PROJECT_CONFIG_OUT "@CMAKE_BINARY_DIR@/include")
include(toolchain/CreateGlobalDefines)

# copy the level files (map packs)
#set(LEVEL_TARGET_PATH "@CMAKE_SOURCE_DIR@/level/")
#include(toolchain/CopyLevelFiles)

#copy the input buttons
set(IMAGESET_TARGET_DIR "@CMAKE_SOURCE_DIR@/cegui/imagesets/")
include(toolchain/CopyButtonImageset)

set(Game_include_directories
    ${PROJECT_CONFIG_OUT}
    ..
    .
    ${BULLET_INCLUDE_DIR}
    ${OIS_INCLUDE_DIR}
    ${OGRE_INCLUDE_DIR}
    ${CEGUI_INCLUDE_DIR}
    ${TINYXML2_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
)
set(Game_link_libraries
    ${BULLET_LIBRARIES}
    ${OIS_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${CEGUI_LIBRARIES}
    ${TINYXML2_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${LUA_LIBRARIES}
)

set(Game_link_libraries
  ${Game_link_libraries}
  pthread)


set(EXECUTABLE_OUTPUT_PATH ../bin)

include_directories(${Game_include_directories})
add_executable(Game ${Game_source_files} ${Game_source_event_files} ${Game_source_xmlresources_files} ${Game_include_files})
target_link_libraries(Game ${Game_link_libraries})

# create source groups
# ==================================================================

function(setup_source_group type)
  foreach(src_file ${ARGN})
    string(REPLACE ${SOURCE_DIR}/ "" src_file ${src_file})
    string(FIND ${src_file} / sep_pos REVERSE)
    if (${sep_pos} EQUAL -1)
      set(group "")
    else()
      string(SUBSTRING ${src_file} 0 ${sep_pos} group)
      string(REPLACE "/" "\\" group ${group})
    endif()
    source_group("${type}\\${group}" FILES ${src_file})
  endforeach()
endfunction(setup_source_group)

setup_source_group(src ${Game_source_files})
setup_source_group(include ${Game_include_files})

# create plugins and resourced file
# ==================================================================

if (APPLE)
  set(PLUGIN_FOLDER "./")
  set(PLUGINS "Plugin=RenderSystem_GL") 

  set(RESOURCES_PREFIX "")
  set(RESOURCES_USING_APK "")
elseif (UNIX)
  find_path(PLUGIN_OPEN_GL_LIB NAMES RenderSystem_GL.so PATH_SUFFIXES Plugins Ogre OGRE lib/OGRE lib/OGRE/Plugins lib/x86_64-linux-gnu/OGRE-1.9.0)
  find_package_handle_standard_args(PLUGIN_OPEN_GL DEFAULT_MSG PLUGIN_OPEN_GL_LIB)
  find_path(PLUGIN_OPEN_GLES2_LIB NAMES RenderSystem_GLES2.so PATH_SUFFIXES Plugins Ogre OGRE lib/OGRE lib/OGRE/Plugins lib/x86_64-linux-gnu/OGRE-1.9.0)
  find_package_handle_standard_args(PLUGIN_OPEN_GLES2 DEFAULT_MSG PLUGIN_OPEN_GLES2_LIB)

  
  set(PLUGINS "")
  if (PLUGIN_OPEN_GL_FOUND)
    set(PLUGIN_FOLDER ${PLUGIN_OPEN_GL_LIB})
    set(PLUGINS "${PLUGINS}Plugin=RenderSystem_GL\n")
  endif()
  if (PLUGIN_OPEN_GLES2_FOUND)
    set(PLUGIN_FOLDER ${PLUGIN_OPEN_GLES2_LIB})
    set(PLUGINS "${PLUGINS}Plugin=RenderSystem_GLES2\n")
  endif()

  set(RESOURCES_PREFIX "../")
  set(RESOURCES_USING_APK "")
else()
  set(PLUGIN_FOLDER "${CMAKE_SOURCE_DIR}/plugins")
  set(PLUGINS "Plugin=RenderSystem_D9")

  set(RESOURCES_PREFIX "../")  
  set(RESOURCES_USING_APK "")
endif()

# release
configure_file("${PROJECT_TEMPLATES_DIR}/plugins.cfg.in" "${EXECUTABLE_OUTPUT_PATH}/plugins.cfg" @ONLY)
configure_file("${PROJECT_TEMPLATES_DIR}/resources.cfg.in" "${EXECUTABLE_OUTPUT_PATH}/resources.cfg" @ONLY)
# debug
configure_file("${PROJECT_TEMPLATES_DIR}/plugins.cfg.in" "${EXECUTABLE_OUTPUT_PATH}/plugins_d.cfg" @ONLY)
configure_file("${PROJECT_TEMPLATES_DIR}/resources.cfg.in" "${EXECUTABLE_OUTPUT_PATH}/resources_d.cfg" @ONLY)


# create project file properties
# ==================================================================

if (APPLE)
  #set_source_file_properties("${Game_source_files}" PROPERTIES COMPILE_FLAGS "-x objective-c++")

  set(CMAKE_CXX_FLAGS "-x objective-c++")

  set_property(TARGET Game PROPERTY MACOSX_BUNDLE TRUE)
  #set_property(TARGET Game PROPERTY MACOSX_BUNDLE_ICON_FILE .icns)

  set_property(TARGET Game PROPERTY MACOSX_BUNDLE_INFO_PLIST "@CMAKE_SOURCE_DIR@/OSX/Game-Info.plist")
  set_property(TARGET Game PROPERTY MACOSX_BUNDLE_GUI_IDENTIFIER "org.mencus.game")
  set_target_properties(Game PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES")

  # here check if ios in ogres cmakelist.txt

  include_directories(
    SYSTEM
    "${OGRE_SDK_ROOT}/include/OGRE"
    "${OGRE_SDK_ROOT}/include/OGRE/OSX"
    "${OGRE_SDK_ROOT}/include/OGRE/Overlay"
    "${OGRE_SDK_ROOT}/include/OGRE/RenderSystems/GL"
    "${OGRE_SDK_ROOT}/include/OGRE/RenderSystems/GL/OSX"
    "${OGRE_SDK_ROOT}/include/OIS"
    "${OGRE_SDK_ROOT}/Dependencies/include"
    "${OGRE_SDK_ROOT}/boost"
    "${CEGUI_SDK_ROOT}/include/cegui-0"
    "${TINYXML2_SDK_ROOT}/include"
    "/opt/local/include"
    )

  target_link_libraries(Game ${CoreVideo_LIBRARIES})

  set_target_properties(Game PROPERTIES LINK_FLAGS "-F /Library/Frameworks -framework IOKIT -framework Cocoa -framework Carbon -framework CoreVideo -framework ApplicationServices -framework Ogre -framework OgreOverlay")

  set_target_properties(Game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
  set(GAME_CONTENTS_PATH ../bin/$(CONFIGURATION)/Game.app/Contents)

  add_custom_command(TARGET Game POST_BUILD
    COMMAND mkdir ARGS -p "${GAME_CONTENTS_PATH}/Frameworks"
    COMMAND mkdir ARGS -p "${GAME_CONTENTS_PATH}/Resources"

    COMMAND ditto ${CEGUI_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    COMMAND ditto ${CEGUI_OGRE_RENDERER_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    COMMAND ditto ${CEGUI_CORE_WINDOW_RENDERER_SET_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    COMMAND ditto ${CEGUI_FREE_IMAGE_IMAGE_CODEC_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    COMMAND ditto ${CEGUI_LIB_XML_PARSER_LIB} ${GAME_CONTENTS_PATH}/Frameworks

    COMMAND ditto ${CEGUI_LIB_XML_PARSER_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    COMMAND ditto ${CEGUI_LIB_XML_PARSER_LIB} ${GAME_CONTENTS_PATH}/Frameworks
    )

  if (TINYXML2_LIBRARIES)
    add_custom_command(TARGET Game POST_BUILD
      COMMAND ditto ${TINYXML2_LIBRARIES} ${GAME_CONTENTS_PATH}/Frameworks)
  endif()

  if (CEGUI_LIB_DBG)
    add_custom_command(TARGET Game POST_BUILD
      COMMAND ditto ${CEGUI_LIB_DBG} ${GAME_CONTENTS_PATH}/Frameworks
      )
  endif()

  # copy plugins and resources config file

  set(RESOURCES_OUTPUT_DIR ${GAME_CONTENTS_PATH}/Resources)
  add_custom_command(TARGET Game POST_BUILD
    COMMAND ditto ${EXECUTABLE_OUTPUT_PATH}/plugins.cfg ${RESOURCES_OUTPUT_DIR}
    COMMAND ditto ${EXECUTABLE_OUTPUT_PATH}/resources.cfg ${RESOURCES_OUTPUT_DIR})
  
  # copy resources

  include(toolchain/CopyResources)

endif()