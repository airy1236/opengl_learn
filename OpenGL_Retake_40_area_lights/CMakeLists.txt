cmake_minimum_required(VERSION 3.2)

project(OpenGL_Retake_40_arae_lights)

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
add_subdirectory(glframework/shader)
add_subdirectory(glframework/mesh)
add_subdirectory(glframework/model)
add_subdirectory(glframework/animator)
add_subdirectory(glframework/animation)
add_subdirectory(glframework/bone)
add_subdirectory(glframework/assimp_glm_helpers)

add_executable(OpenGL_Retake_40_area_lights "main.cpp" "glad.c")

target_link_libraries(OpenGL_Retake_40_area_lights glfw3.lib assimp-vc143-mtd.lib freetype.lib cm sh me mo app ant an agh bo)