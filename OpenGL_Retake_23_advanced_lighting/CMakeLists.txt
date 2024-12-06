cmake_minimum_required(VERSION 3.2)

project(OpenGL_Retake_23_advanced_lighting)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ASSETS "./assets")

file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib
)

add_subdirectory(application)
add_subdirectory(glframework/camera)
add_subdirectory(glframework/texture)
add_subdirectory(glframework/shader)
add_subdirectory(glframework/mesh)
add_subdirectory(glframework/model)

add_executable(OpenGL_Retake_23_advanced_lighting "main.cpp" "glad.c")

target_link_libraries(OpenGL_Retake_23_advanced_lighting glfw3.lib assimp-vc143-mtd.lib cm tx sh me mo app)