project(Win32App)

include_directories(${3RDPARTY_DIR}/imgui)
include_directories(${3RDPARTY_DIR}/imgui/backends)
include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${ENGINE_DIR}/core/timer)
include_directories(${ENGINE_DIR}/core/input)
include_directories(${ENGINE_DIR}/core/graphics)

set(ALL_FILE
    main.cpp
    2.cpp
    Win32App.hpp
    Win32App.cpp
)

add_executable(${PROJECT_NAME}_Sample WIN32 ${ALL_FILE})

target_link_libraries(
    ${PROJECT_NAME}_Sample
    IMGUI_WIN32_DX12
    Timer
    GraphicsManager
)

set_target_properties(${PROJECT_NAME}_Sample PROPERTIES FOLDER "samples")
