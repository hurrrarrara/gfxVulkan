cmake_minimum_required(VERSION 3.10.0)


#####################
# SHADER COMPILATION
#####################

find_program(SLANGC slangc REQUIRED)

set(SHADER_SOURCES_DIR ${CMAKE_SOURCE_DIR}/ressources/shaders)
set(SHADER_SOURCES
	${SHADER_SOURCES_DIR}/default.vert.slang
	${SHADER_SOURCES_DIR}/grid.vert.slang
	${SHADER_SOURCES_DIR}/face_color.frag.slang
	${SHADER_SOURCES_DIR}/grid.frag.slang

)

set(SHADER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/ressources/compiled_shaders)
set(SHADER_SPV_FILES)

foreach(SHADER ${SHADER_SOURCES})
	get_filename_component(FILE_NAME ${SHADER} NAME)
	set(SPV ${SHADER_OUTPUT_DIR}/${FILE_NAME}.spv)

	list(APPEND SHADER_SPV_FILES ${SPV})

	add_custom_command(
		OUTPUT ${SPV}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIR}
		# COMMAND slangc ${SHADER} -lang slang -target spirv -profile glsl_450 -emit-spirv-directly -fvk-use-entrypoint-name -fvk-use-gl-layout ${ENTRY_POINTS} -o ${SPV}
		COMMAND slangc ${SHADER} -lang slang -target spirv -profile glsl_450 -emit-spirv-directly -fvk-use-gl-layout -o ${SPV}
		DEPENDS ${SHADER}
		VERBATIM
	)
endforeach()

add_custom_target(shaders DEPENDS ${SHADER_SPV_FILES})

##############
# PROJECT
##############


project(
  Renderer
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()


add_subdirectory(external/Vulkan-Headers)
# add_subdirectory(external/glm)

# to reduce compile by not bulding unescessary stuff
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

add_subdirectory(gfx_vulkan)
add_subdirectory(window)
# add_subdirectory(gfx_ecs)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# add_dependencies(${PROJECT_NAME} shaders)

# target_link_libraries(Renderer PRIVATE gfx_vulkan gfx_window gfx_ecs glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE window)
target_compile_definitions(${PROJECT_NAME} PRIVATE
	SHADERS_DIR="${SHADER_OUTPUT_DIR}"
	MODELS_DIR="${CMAKE_SOURCE_DIR}/ressources/models"
)

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E create_symlink
#             $<TARGET_FILE:${PROJECT_NAME}>
# 			${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}
# )

set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(EXAMPLES_SOURCES
	${EXAMPLES_DIR}/window_creation.cpp
	${EXAMPLES_DIR}/device_creation.cpp

)
foreach(SOURCE ${EXAMPLES_SOURCES})
	get_filename_component(FILE_NAME ${SOURCE} NAME_WE)
	add_executable(${FILE_NAME} ${SOURCE})
	target_link_libraries(${FILE_NAME} PRIVATE window gfx_vulkan)
endforeach()

#
# #####################
# #  X11 for Renderdoc
# #####################
# set(PROJECT_NAME_X11 ${PROJECT_NAME}_x11)
#
# add_executable(${PROJECT_NAME_X11} EXCLUDE_FROM_ALL)
# target_sources(${PROJECT_NAME_X11} PRIVATE ${SOURCES})
#
# add_dependencies(${PROJECT_NAME_X11} shaders)
#
# target_link_libraries(${PROJECT_NAME_X11} PRIVATE gfx_vulkan gfx_window_x11 glm::glm)
#
# target_compile_definitions(${PROJECT_NAME_X11} PRIVATE
#     SHADERS_DIR="${SHADER_OUTPUT_DIR}"
# 	MODELS_DIR="${CMAKE_SOURCE_DIR}/ressources/models"
# )
#
# add_custom_command(TARGET ${PROJECT_NAME_X11} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E create_symlink
#             $<TARGET_FILE:${PROJECT_NAME_X11}>
#             ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME_X11}
# )

