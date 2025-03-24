cmake_minimum_required(VERSION 3.30)
project(C_Vent VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(C_Vent STATIC modules/event/include/event.h)

set_target_properties(C_Vent PROPERTIES LINKER_LANGUAGE CXX)

find_package(Catch2 3 REQUIRED)

add_subdirectory(tests)

target_include_directories(C_Vent
        PUBLIC
            ${CMAKE_SOURCE_DIR}/modules/event/include
            ${CMAKE_SOURCE_DIR}/modules/event/impl)