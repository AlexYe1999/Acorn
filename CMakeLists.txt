cmake_minimum_required(VERSION 3.10.0)
project(AcornEngine)

option(PLATFORM_LINUX "option for linux" OFF)
option(PLATFORM_WINDOWS "option for windows" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

include(CMakeDependentOption)

if(PLATFORM_WINDOWS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    add_compile_definitions("PLATFORM_WINDOWS=true")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(engine)
