cmake_minimum_required(VERSION 3.10)

project(HelloVulkanGameEngine)
file(GLOB_RECURSE HVGE_SOURCES "${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/src/*.cpp")
file(GLOB_RECURSE HVGE_HEADERS "${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/src/*.h")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/src")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HelloVulkanGameEngine)
set (CMAKE_CXX_STANDARD 17)
add_executable(HelloVulkanGameEngine ${HVGE_SOURCES} ${HVGE_HEADERS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/vendor/glfw")      
target_link_libraries(HelloVulkanGameEngine glfw)

add_subdirectory("${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/vendor/glm")
include_directories("${CMAKE_SOURCE_DIR}/HelloVulkanGameEngine/vendor/glm")

set(VULKAN_PATH "C:/VulkanSDK/1.2.170.0")                                             
include_directories(AFTER "${VULKAN_PATH}/Include")
target_link_libraries(HelloVulkanGameEngine "${VULKAN_PATH}/Lib/vulkan-1.lib")
