cmake_minimum_required(VERSION 3.10.0)
project(AcornEngine)

option(PLATFORM_LINUX "option for linux" OFF)
option(PLATFORM_WINDOWS "option for windows" OFF)

set(CMAKE_CXX_STANDARD 17)

if(PLATFORM_WINDOWS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine CACHE PATH "engine folder dir")
set(SAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/samples CACHE PATH "samples folder dir")
set(3RDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty CACHE PATH "3rdparty folder dir")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(engine)
add_subdirectory(samples)
add_subdirectory(3rdparty)