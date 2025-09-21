cmake_minimum_required(VERSION 3.5)
project(drawww)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(vendor/glfw)

# GLAD setup
set(GLAD_SOURCES "src/gl.c")

# Source files
set(APP_SRC main.cpp)

file(GLOB_RECURSE APP_SRC_LIB "src/*.cpp")

# Create executable
add_executable(drawww ${APP_SRC} ${APP_SRC_LIB} ${GLAD_SOURCES})

# Set C++ standard for this specific target
target_compile_features(drawww PRIVATE cxx_std_17)

# Compile the target with debug information
target_compile_options(drawww PRIVATE -g -O0 -fno-omit-frame-pointer)

# Link libraries and set includes
target_link_libraries(drawww ${OPENGL_LIBRARIES} glfw)
target_include_directories(drawww PRIVATE ${OPENGL_INCLUDE_DIRS} vendor/glad)