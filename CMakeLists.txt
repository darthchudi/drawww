cmake_minimum_required(VERSION 3.5)
project(drawww)

if (EMSCRIPTEN)
    # WASM build
    # If building with Emscripten, clear any inherited macOS arch flags early
    set(CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)

    # Source files
    set(APP_SRC main.cpp)
    file(GLOB_RECURSE APP_SRC_LIB "src/*.cpp")

    # Include GLAD loader so symbol stubs (glad_*) resolve under WebGL
    set(GLAD_SOURCES "src/gl.cpp")

    add_executable(drawww ${APP_SRC} ${APP_SRC_LIB} ${GLAD_SOURCES})
    target_compile_features(drawww PRIVATE cxx_std_17)
    target_include_directories(drawww PRIVATE vendor/glad)

    # Link with Emscripten WebGL/GLFW shims
    target_link_options(drawww PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASSERTIONS=1"
        "-sWASM=1"
        "-sGL_ENABLE_GET_PROC_ADDRESS=1"
        "--preload-file=${CMAKE_SOURCE_DIR}/src/shaders@/shaders" # Preload the shaders
    )

    # Produce a JS wasm output
    set_target_properties(drawww PROPERTIES SUFFIX ".js")

else()
    # Native build
    find_package(OpenGL REQUIRED)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
    set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
    add_subdirectory(vendor/glfw)

    # GLAD setup
    set(GLAD_SOURCES "src/gl.cpp")

    # Source files
    set(APP_SRC main.cpp)
    file(GLOB_RECURSE APP_SRC_LIB "src/*.cpp")

    # Create executable
    add_executable(drawww ${APP_SRC} ${APP_SRC_LIB} ${GLAD_SOURCES})

    # Set C++ standard for this specific target
    target_compile_features(drawww PRIVATE cxx_std_17)

    # Compile the target with debug information
    target_compile_options(drawww PRIVATE -g -O0 -fno-omit-frame-pointer)

    # Set includes and Link libraries
    target_include_directories(drawww PRIVATE ${OPENGL_INCLUDE_DIRS} vendor/glad)
    target_link_libraries(drawww ${OPENGL_LIBRARIES} glfw)
endif()