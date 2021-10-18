project(TextureMapping)

include_directories(${SAMPLES_DIR}/AppFramework)
include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${ENGINE_DIR}/core/timer)
include_directories(${ENGINE_DIR}/core/camera)
include_directories(${ENGINE_DIR}/core/input)
include_directories(${ENGINE_DIR}/core/graphics/D3DGraphics)
include_directories(${ENGINE_DIR}/core/graphics/D3DGraphics/resource)

set(ALL_FILE
    TextureApp.hpp
    TextureApp.cpp
    Waves.h
    Waves.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${ALL_FILE})

target_link_libraries(
    ${PROJECT_NAME}
    AppFramework
    Timer
    Input
    GraphicsManager
    d3dcompiler
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "samples")
