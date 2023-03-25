
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(3RDPARTY_DIR "${ENGINE_ROOT_DIR}/3rdparty")
set(ENGINE_ASSET_DIR "/asset")

add_compile_definitions(
    "ENGINE_ROOT_DIR=${ENGINE_ROOT_DIR}"
    "NOMINMAX"
)

if(PLATFORM_WINDOWS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/MP")
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

add_subdirectory(source/3rdparty)
add_subdirectory(source/tools)
add_subdirectory(source/runtime)
add_subdirectory(source/editor)