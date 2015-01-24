package org.cwdev.zelda;

public final class GlobalBuildDefines {
    // define version
    public final static int PROJECT_VERSION_CODE = 1;
    public final static String PROJECT_VERSION_NAME = "0.0.1";

    // Flag to define whether to include the map editor
    public final static boolean ENABLE_MAP_EDITOR = 0 == 1;
    
    // Flag to define whether to enable the campaign
    public final static boolean ENBLE_CAMPAIGN = 0 == 1;

    // Flag to define whether to enable cheats
    public final static boolean ENABLE_CHEATS = 0 == 1;

    // Flag to define wheter to enable touch-screen input
    public final static boolean ENABLE_INPUT_TOUCH = 1 == 1;

    // Index of the input platform (0: keyboard, 1: touch, 2: amazon remote control)
    public final static int PROJECT_INPUT_PLATFORM = 1;

    // define whether there are input buttons
    public final static boolean PROJECT_HAS_INPUT_BUTTONS_IMAGESET = PROJECT_INPUT_PLATFORM == 0 || PROJECT_INPUT_PLATFORM == 2;

    // Flag to enable debug output
    public final static boolean ENABLE_DEBUG_DEFINES = 0 == 1;

    // Enable Shader manager (required for GLESES, optional for OpenGL/DirectX)
    public final static boolean PROJECT_ENABLE_SHADER = 1 == 1;

    // Assert mode
    // 0: not asserting (release)
    // 1: asserts (debug)
    public final static int PROJECT_ASSERT_MODE = 1;

    // Ads enabled
    public final static boolean PROJECT_ENABLE_ADS = 0 == 1;

    // use amazon game circle
    public final static boolean PROJECT_USE_AMAZON_GAME_CIRCLE = 0 == 1;

    // default login to social gaming
    public final static boolean PROJECT_DEFAULT_LOGIN_TO_SOCIAL_GAMING = 0 == 1;
}
