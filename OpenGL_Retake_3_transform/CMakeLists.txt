cmake_minimum_required(VERSION 3.12)

project(OpenGL_Retake_3_transform)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ASSETS "./assets")

file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories (
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include
)

link_directories (
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib
)

add_subdirectory(glframework)
add_subdirectory(application)

add_executable(OpenGL_Retake_3_transform "main.cpp" "glad.c")

target_link_libraries(OpenGL_Retake_3_transform glfw3.lib app fw)