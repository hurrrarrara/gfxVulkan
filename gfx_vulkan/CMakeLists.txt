cmake_minimum_required(VERSION 3.10.0)

set(PROJECT_NAME vulkanLib)
set(LIBRARY_NAME gfx_vulkan)
set(LIBRARY_HEADERS_DIR include/${LIBRARY_NAME})
set(LIBRARY_HEADERS 
	${LIBRARY_HEADERS_DIR}/Device.h
	${LIBRARY_HEADERS_DIR}/BufferPool.h
	${LIBRARY_HEADERS_DIR}/details/prerequisites.h
	${LIBRARY_HEADERS_DIR}/details/log.h
)
set(LIBRARY_SOURCE_DIR src)
set(LIBRARY_SOURCE 
	${LIBRARY_SOURCE_DIR}/Device.cpp
	${LIBRARY_SOURCE_DIR}/utils.h
	${LIBRARY_SOURCE_DIR}/init/instance.cpp
	${LIBRARY_SOURCE_DIR}/init/physical_device.cpp
	${LIBRARY_SOURCE_DIR}/init/device.cpp
)

project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

# target_include_directories(${LIBRARY_NAME} PRIVATE
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
#     $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
# )
#
# target_include_directories(${LIBRARY_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:include>
# )

target_include_directories(${LIBRARY_NAME}
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${LIBRARY_NAME} PUBLIC 
	Vulkan::Headers
	GPUOpen::VulkanMemoryAllocator
)
