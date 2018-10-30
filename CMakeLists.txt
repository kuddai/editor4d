cmake_minimum_required(VERSION 3.5)
project(editor4d)

add_definitions(-std=c++11)
set (CMAKE_CXX_STANDARD 11)

# header files
include_directories(include lib lib/catch2)

file(GLOB SOURCES src/core/*.cpp)
file(GLOB TESTS tests/*.cpp)

# library that will be used for different targets
# (tests and browser). NOTE: couldn't link SDL to tests,
# so no opengl there
add_library(editor4d ${SOURCES})

# tests
add_executable(test_editor4d ${TESTS} lib/catch2/catch_main.cpp)
target_link_libraries(test_editor4d editor4d)

if (DEFINED EMSCRIPTEN)
    # browser
    add_executable(main ${SOURCES})
    target_link_libraries(main editor4d)

    set(LINK_FLAGS "-s WASM=1 -s USE_SDL=2 -O3")
    set_target_properties(main PROPERTIES OUTPUT_NAME "index" SUFFIX ".js" LINK_FLAGS ${LINK_FLAGS})
    em_link_js_library(index ${libraryJsFiles})
endif (DEFINED EMSCRIPTEN)
