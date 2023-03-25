set(TARGET_NAME AcornRuntime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES "*.hpp")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(Vulkan)
target_include_directories(${TARGET_NAME}
PUBLIC 
    ${Vulkan_INCLUDE_DIR} 
    ${ENGINE_ROOT_DIR}/source
)
target_link_libraries(${TARGET_NAME} 
PUBLIC 
    ${Vulkan_LIBRARY}
    glfw
    spdlog
    nlohmann_json::nlohmann_json
    dxgi
    d3d12
    Tracy::TracyClient
    EnTT
)

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "engine/runtime")

target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")