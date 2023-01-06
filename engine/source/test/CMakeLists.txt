
add_compile_definitions("ENGINE_ROOT_DIR=${ENGINE_ROOT_DIR}")

add_executable(glfwTest glfw.cpp)
target_include_directories(glfwTest PRIVATE ${ENGINE_ROOT_DIR}/source)
target_link_libraries(glfwTest AcornRuntime)

if(PLATFORM_WINDOWS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/MP")
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

    add_executable(Win32Test WIN32 win32.cpp)      
    target_include_directories(Win32Test PRIVATE ${ENGINE_ROOT_DIR}/source)
    target_link_libraries(Win32Test AcornRuntime)

endif()


