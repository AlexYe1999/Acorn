
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ENGINE_ASSET_DIR "${ENGINE_ROOT_DIR}/asset")

if(PLATFORM_WINDOWS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/MP")
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    add_compile_definitions("PLATFORM_WINDOWS=true")
endif()

add_subdirectory(source/3rdparty)
add_subdirectory(source/runtime)
add_subdirectory(source/test)