if (NOT DEFINED PROJECT_INPUT_PLATFORM)
  set(PROJECT_INPUT_PLATFORM 0 CACHE INTEGER "0: Keyboard, 1: Touch, 2: Amazon Remote Control")
endif()

if (NOT DEFINED PROJECT_USE_AMAZON_GAME_CIRCLE)
  set(PROJECT_USE_AMAZON_GAME_CIRCLE 0 CACHE INTEGER "Use the amazon game circle")
endif()

if (NOT DEFINED PROJECT_DEFAULT_LOGIN_TO_SOCIAL_GAMING)
  set(PROJECT_DEFAULT_LOGIN_TO_SOCIAL_GAMING 0 CACHE INTEGER "Login to the selected social gaming platform by default")
endif()

if (NOT DEFINED PROJECT_ENABLE_MAP_EDITOR)
  set(PROJECT_ENABLE_MAP_EDITOR 1 CACHE INTEGER "Enable the mapeditor")
endif()

if (NOT DEFINED PROJECT_ENABLE_CAMPAIGN)
  set(PROJECT_ENABLE_CAMPAIGN 0 CACHE INTEGER "Enable campaigns")
endif()

if (NOT DEFINED PROJECT_ENABLE_CHEATS)
  set(PROJECT_ENABLE_CHEATS 0 CACHE INTEGER "Enable cheats")
endif()

if (NOT DEFINED PROJECT_ENABLE_INPUT_TOUCH)
  set(PROJECT_ENABLE_INPUT_TOUCH 0 CACHE INTEGER "Enable touch input")
endif()

if (NOT DEFINED PROJECT_ENABLE_DEBUG_DEFINES)
  set(PROJECT_ENABLE_DEBUG_DEFINES 0 CACHE INTEGER "Enable debug defines, for displaying debug output")
endif()

if (NOT DEFINED PROJECT_ENABLE_SHADER)
  set(PROJECT_ENABLE_SHADER 0 CACHE INTEGER "Use shaders (required for GLSLES)")
endif()

if (NOT DEFINED PROJECT_ASSERT_MODE)
  set(PROJECT_ASSERT_MODE 1 CACHE INTEGER "0: no asserts, 1: use asserts")
endif()

if (NOT DEFINED PROJECT_ENABLE_ADS)
  set(PROJECT_ENABLE_ADS 0 CACHE INTEGER "0: no ads, 1: use ads")
endif()

if (NOT DEFINED PROJECT_CONFIG_OUT) 
  Message("Please set the PROJECT_CONFIG_OUT variable to an apropriate path and add it to the include path")
else()
  file(MAKE_DIRECTORY ${PROJECT_CONFIG_OUT})

  configure_file("${PROJECT_TEMPLATES_DIR}/GlobalBuildDefines.hpp.in" "${PROJECT_CONFIG_OUT}/GlobalBuildDefines.hpp" @ONLY)
  
  if (ANDROID)
    configure_file("${PROJECT_TEMPLATES_DIR}/GlobalBuildDefines.java.in" "${NDKOUT}/src/org/${LOWERCASE_PROJECT_NAME}/GlobalBuildDefines.java" @ONLY)
  endif()

endif()


# addidtional global properties
if (PROJECT_USE_AMAZON_GAME_CIRCLE)
  include(toolchain/AmazonGameCircle)
endif()