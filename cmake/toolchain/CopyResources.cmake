if (NOT RESOURCES_OUTPUT_DIR) 
  message(fatal_error "RESOURCES_OUTPUT_DIR was not set")
endif()

# create packs

add_custom_command(TARGET Game POST_BUILD
	COMMAND python ARGS "${CMAKE_SOURCE_DIR}/tools/CreatePacks.py")

# define copy functions

if (APPLE)
	function(copy_directory directory)
		add_custom_command(TARGET Game POST_BUILD
			COMMAND ditto "${RESOURCES_ROOT_DIR}/${directory}" "${RESOURCES_OUTPUT_DIR}/${directory}")
	endfunction(copy_directory)

	function(copy_files directory file)
		add_custom_command(TARGET Game POST_BUILD
	    	COMMAND mkdir ARGS -p "${RESOURCES_OUTPUT_DIR}/${directory}")

		file(GLOB files "${RESOURCES_ROOT_DIR}/${directory}${file}")
		foreach(single_file ${files})
			add_custom_command(TARGET Game POST_BUILD
				COMMAND ditto ${single_file} ${RESOURCES_OUTPUT_DIR}/${directory}/
				)
		endforeach(single_file)
	endfunction(copy_files)

else()
	function(copy_directory directory)
		file(COPY "${RESOURCES_ROOT_DIR}/${directory}" DESTINATION "${RESOURCES_OUTPUT_DIR}/${directory}")
	endfunction(copy_directory)

	function(copy_files directory file)
		file(GLOB files "${RESOURCES_ROOT_DIR}/${directory}${file}")
		file(COPY ${files} DESTINATION ${RESOURCES_OUTPUT_DIR}/${directory})
	endfunction(copy_files)
endif()


set (RESOURCES_ROOT_DIR ${CMAKE_SOURCE_DIR})

copy_directory("cegui")
copy_directory("RTShaderLib")
copy_files("packs/" "*.zip")
copy_files("maps/Atlases/LightWorld/" "*.zip")