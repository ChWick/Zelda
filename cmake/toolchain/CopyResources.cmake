if (NOT RESOURCES_OUTPUT_DIR) 
  message(fatal_error "RESOURCES_OUTPUT_DIR was not set")
endif()

if (APPLE)
	function(copy_directory directory file)
		add_custom_command(TARGET Game POST_BUILD
	    	COMMAND mkdir ARGS -p "${RESOURCES_OUTPUT_DIR}/${directory}")

		file(GLOB files "${RESOURCES_ROOT_DIR}/${directory}${file}")
		foreach(single_file ${files})
			add_custom_command(TARGET Game POST_BUILD
				COMMAND ditto ${single_file} ${RESOURCES_OUTPUT_DIR}/${directory}${file}
				)
		endforeach(single_file)
	endfunction(copy_directory)

else()
	function(copy_directory directory file)
		file(GLOB files "${RESOURCES_ROOT_DIR}/${directory}${file}")
		file(COPY ${files} DESTINATION ${RESOURCES_OUTPUT_DIR}/${directory})
	endfunction(copy_directory)
endif()


set (RESOURCES_ROOT_DIR ${CMAKE_SOURCE_DIR})

copy_directory("" "cegui")
copy_directory("" "RTShaderLib")
copy_directory("packs/" "*.zip")
copy_directory("maps/Atlases/LightWorld/" "*.zip")