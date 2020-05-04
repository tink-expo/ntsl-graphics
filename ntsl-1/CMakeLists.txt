cmake_minimum_required(VERSION 3.8)

# if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#   set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE PATH "toolchain file")
# endif()

project(CS380)

add_compile_options ( "-std=c++14" )

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
set(GLM_INCLUDE_DIRS /usr/local/include/glm)

set(ENV{ROOT_SRC} /Users/woohyunhan/ntsl-graphics/ntsl-1)

add_subdirectory(Engine)
add_subdirectory(Application)