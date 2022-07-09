
add_executable(GLTFTest glfw.cpp)

set(Wind32Test "Wind32Test")
add_executable(${Wind32Test} WIN32 win32.cpp)

target_include_directories(${Wind32Test} PRIVATE ${ENGINE_ROOT_DIR}/source)
target_link_libraries(${Wind32Test} AcornRuntime)