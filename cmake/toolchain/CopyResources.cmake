if (NOT RESOURCES_OUTPUT_DIR) 
  message(fatal_error "RESOURCES_OUTPUT_DIR was not set")
endif()

set (RESOURCES_ROOT_DIR ${CMAKE_SOURCE_DIR})

file(COPY "${RESOURCES_ROOT_DIR}/cegui" DESTINATION ${RESOURCES_OUTPUT_DIR})

file(GLOB Packs "${RESOURCES_ROOT_DIR}/packs/*.zip")
file(COPY ${Packs} DESTINATION  "${RESOURCES_OUTPUT_DIR}/packs")
file(COPY "${RESOURCES_ROOT_DIR}/RTShaderLib" DESTINATION "${RESOURCES_OUTPUT_DIR}")

file(GLOB LightWorldMaps "${RESOURCES_ROOT_DIR}/maps/Atlases/LightWorld/*.zip")
file(COPY ${LightWorldMaps} DESTINATION "${RESOURCES_OUTPUT_DIR}/maps/Atlases/LightWorld")