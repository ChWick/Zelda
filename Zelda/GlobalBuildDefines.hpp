#ifndef _GLOBAL_BUILD_DEFINES_HPP_
#define _GLOBAL_BUILD_DEFINES_HPP_

// define version
#define MENCUS_VERSION_CODE   12
#define MENCUS_VERSION_NAME   "0.0.12"
#define PROJECT_NAME "Zelda"

// Flag to define whether to include the map editor
#define ENABLE_MAP_EDITOR     0

// Flag to define whether to enable the campaign
#define ENABLE_CAMPAIGN       0

// Flag to define whether to enable cheats
#define ENABLE_CHEATS         0

// Flag to define wheter to enable touch-screen input
#define ENABLE_INPUT_TOUCH    0

// Index of the input platform (0: keyboard, 1: touch, 2: amazon remote control)
#define MENCUS_INPUT_PLATFORM 0

// define whether there are input buttons
#if MENCUS_INPUT_PLATFORM == 0 || MENCUS_INPUT_PLATFORM == 2
#define MENCUS_HAS_INPUT_BUTTONS_IMAGESET 1
#else
#define MENCUS_HAS_INPUT_BUTTONS_IMAGESET 0
#endif

// Flag to enable debug output
#define ENABLE_DEBUG_DEFINES  0

// Enable Shader manager (required for GLESES, optional for OpenGL/DirectX)
#define MENCUS_ENABLE_SHADER  1

// Assert mode
// 0: not asserting (release)
// 1: asserts (debug)
#define PROJECT_ASSERT_MODE    1

// Ads enabled
#define MENCUS_ENABLE_ADS     0

// use amazon game circle
#define MENCUS_USE_AMAZON_GAME_CIRCLE    0

// default login to social gaming
#define MENCUS_DEFAULT_LOGIN_TO_SOCIAL_GAMING 0

#endif
