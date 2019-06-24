cmake_minimum_required(VERSION 3.5)
project(editor4d)

add_definitions(-std=c++11)
set (CMAKE_CXX_STANDARD 11)

# Header files.
# TODO remove include_directories and create separate CMakeLists.txt for
# third_party.
# More info can be found here:
# https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
# https://github.com/pabloariasal/modern-cmake-sample/
# https://gitlab.com/slon/shad-cpp/blob/master/CMakeLists.txt
# add_subdirectory(third_party/Eigen)
include_directories(core third_party)

file(GLOB SOURCES core/*.cpp)
# Filter out unittests. More about regex filtering can be found here:
# https://stackoverflow.com/questions/42084627/cmake-how-to-exclude-files-in-macos-in-fileglob-directive
list(FILTER SOURCES EXCLUDE REGEX ".+_unittest\\.cpp$")
# Exclude executable from library definiton.
list(FILTER SOURCES EXCLUDE REGEX "editor4d_wasm\\.cpp$")

add_library(editor4d ${SOURCES})

# TODO add unittests target with googletests (current test are written with
# catch)

if (DEFINED EMSCRIPTEN)
  # Emscripten build for browser..
  add_executable(editor4d_wasm core/editor4d_wasm.cpp)
  target_link_libraries(editor4d_wasm editor4d)

  # More information about emscripten compilation options can be found here:
  # https://emscripten.org/docs/getting_started/FAQ.html
  string(CONCAT LINK_FLAGS
    # Configuration for Debugging.
    " -g" 
    " -O0"
    # Specifies that we want wasm output. If we don’t
    # specify this, Emscripten will just output asm.js,
    # as it does by default.
    " -s WASM=1"
    # Error in linkage will result in error in compile time.
    " -s ERROR_ON_UNDEFINED_SYMBOLS=0"
    # Wasm Module will provide Promise like interface with "then".
    " -s MODULARIZE=1"
    # The function for module creation with "ModuleOverrides" option exposed on
    # window object. Returns @type {Promise<Module>}.
    " -s EXPORT_NAME=\"'_createEditor4d'\"")

  # Generates index.js which wraps and loads *.wasm file.
  set_target_properties(editor4d_wasm PROPERTIES
    OUTPUT_NAME "index"
    SUFFIX ".js"LINK_FLAGS ${LINK_FLAGS})
endif (DEFINED EMSCRIPTEN)
