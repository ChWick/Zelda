if (NOT IMAGESET_TARGET_DIR)
  message(FATAL_ERROR "IMAGESET_TARGET_DIR not set")
endif()

set(IMAGESET_SRC_DIR "${CMAKE_SOURCE_DIR}/ResourcesJar/Buttons/")


function(copy_button_imageset filename)
  configure_file("${IMAGESET_SRC_DIR}${filename}.png" "${IMAGESET_TARGET_DIR}input_buttons.png" COPYONLY)
  configure_file("${IMAGESET_SRC_DIR}${filename}.imageset" "${IMAGESET_TARGET_DIR}input_buttons.imageset")
endfunction()

if (PROJECT_INPUT_PLATFORM EQUAL 0)
  copy_button_imageset(buttons_keyboard)
elseif (PROJECT_INPUT_PLATFROM EQUAL 1)
  # copy nothing, touch device with buttons already included
elseif (PROJECT_INPUT_PLATFORM EQUAL 2)
  copy_button_imageset(buttons_amazon_remote_control)
else() 
  message(FATAL_ERROR "Unknown input platform")
endif()