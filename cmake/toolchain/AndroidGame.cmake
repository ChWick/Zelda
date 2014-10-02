if (ANDROID)  
  # create the GlobalBuildDefines file
  set (PROJECT_CONFIG_OUT "@CMAKE_BINARY_DIR@/include")
  set(NDKOUT "${CMAKE_BINARY_DIR}/project")
  set(PROJECT_ENABLE_MAP_EDITOR 0)
  set(PROJECT_ENABLE_SHADER 1)
  include(toolchain/CreateGlobalDefines)

  if(APPLE OR WIN32)
    SET(ANDROID_EXECUTABLE "android")
    SET(NDK_BUILD_EXECUTABLE "ndk-build")
  else()
    SET(ANDROID_EXECUTABLE "$ENV{ANDROID_SDK}/tools/android")
    SET(NDK_BUILD_EXECUTABLE "$ENV{ANDROID_NDK}/ndk-build")
  endif()

  SET(ANDROID_MOD_NAME ${CMAKE_PROJECT_NAME})
  SET(DEPENDENCIES_LDLIBS "")
  SET(SOURCE_DIR "@CMAKE_SOURCE_DIR@/${GAME_DIR}")
  FILE(GLOB_RECURSE GAME_SOURCE_FILES "${GAME_DIR}/*.cpp")
  STRING (REPLACE ";" "\nLOCAL_SRC_FILES += " GAME_SOURCE_FILES "${GAME_SOURCE_FILES}")
  SET(SOURCE_FILES ${GAME_SOURCE_FILES})
  SET(ANT_EXECUTABLE "ant")
  
  if(${ANDROID_NATIVE_API_LEVEL} LESS 14)
    MATH(EXPR ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}+1")
  else()
    SET(ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}")
    #SET(SCREEN_SIZE "|screenSize")
  endif()
  
  if(OGRE_CONFIG_ENABLE_GLES2_GLSL_OPTIMISER)
    SET(GLES_OPTIMISER "-lglsl_optimizer -lmesa -lglcpp-library")
  endif()

  SET(ANDROID_TARGET "android-${ANDROID_SDK_API_LEVEL}")

  file(MAKE_DIRECTORY "${NDKOUT}")
  file(MAKE_DIRECTORY "${NDKOUT}/jni")
  file(MAKE_DIRECTORY "${NDKOUT}/assets")
  
  # we need network access if using the amazon game circle
  if (PROJECT_ENABLE_ADS OR PROJECT_USE_AMAZON_GAME_CIRCLE) 
    SET(PROJECT_ANDROID_PERMISSIONS
      "<uses-permission android:name=\"android.permission.INTERNET\" /> 
  <!-- Used to avoid sending an ad request if there is no connectivity. -->
  <uses-permission android:name=\"android.permission.ACCESS_NETWORK_STATE\" />")
  endif()

  file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")
  file(WRITE "${NDKOUT}/jni/Application.mk" "NDK_TOOLCHAIN_VERSION := 4.8\nAPP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")
  configure_file("${PROJECT_TEMPLATES_DIR}/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)

  configure_file("${PROJECT_TEMPLATES_DIR}/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

  # resources file
  #SET(RESOURCES_MINIMAL "Minimal")
  SET(RESOURCES_USING_APK "APK")
  SET(RESOURCES_PREFIX "")

  configure_file("${PROJECT_TEMPLATES_DIR}/resources.cfg.in" "${NDKOUT}/assets/resources.cfg" @ONLY)

  set(RESOURCES_OUTPUT_DIR "${NDKOUT}/assets")
  include(toolchain/CopyResources)

  add_custom_command(
    TARGET Game
    POST_BUILD
    COMMAND ${ANDROID_EXECUTABLE} update project --target ${ANDROID_TARGET} --path "${NDKOUT}"
    WORKING_DIRECTORY ${NDKOUT}
    )

endif()
