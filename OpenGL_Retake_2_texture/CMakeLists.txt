cmake_minimum_required(VERSION 3.12)

project(OpenGL_Retake_2_texture)

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
add_subdirectory(glframework)

add_executable(OpenGL_Study "main.cpp" "glad.c")

target_link_libraries(OpenGL_Study glfw3.lib app fw)