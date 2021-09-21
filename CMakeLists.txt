cmake_minimum_required(VERSION 3.10.0)
project(AcornEngine)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine CACHE PATH "engine folder dir")
set(3RDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty CACHE PATH "3rdparty folder dir")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(scripts/InitResource.cmake)

InitResource()

set(IMGUI_URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.84.2.zip")
set(IMGUI_FILE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui)

if(NOT EXISTS ${IMGUI_FILE})
    DownloadFile(${IMGUI_URL} ${IMGUI_FILE}.zip)
    #if(EXISTS ${IMGUI_FILE}.zip)
        #ExtractFile(${IMGUI_FILE}.zip ${IMGUI_FILE})
    #else(EXISTS ${IMGUI_FILE}.zip)
        #message(ERROR "can not find " ${IMGUI_FILE}.zip) 
    #endif(EXISTS ${IMGUI_DIR}.zip)
endif(NOT EXISTS ${IMGUI_FILE})

add_subdirectory(engine)
add_subdirectory(samples)
add_subdirectory(3rdparty)