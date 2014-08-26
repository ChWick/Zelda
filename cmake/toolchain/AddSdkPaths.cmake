if (NOT DEFINED OGRE_SDK_ROOT)
  set(OGRE_SDK_ROOT "" CACHE PATH "Path to the ogre sdk/install directory")
endif()

if (NOT DEFINED CEGUI_SDK_ROOT)
  set(CEGUI_SDK_ROOT "" CACHE PATH "Path to the cegui sdk/install directory")
endif()

if (NOT DEFINED TINYXML2_SDK_ROOT)
  set(TINYXML2_SDK_ROOT "" CACHE PATH "Path to the tinyxml2 sdk/install directoy")
endif()

if (NOT DEFINED BOOST_ROOT)
  set(BOOST_ROOT "" CACHE PATH "Path to the boost sdk/install directoy")
endif()

if (NOT DEFINED OGRE_DEPS_ROOT)
  set(OGRE_DEPS_ROOT ${OGRE_SDK_ROOT}/dependencies CACHE PATH "Path to the ogre dependencies directoy")
endif()