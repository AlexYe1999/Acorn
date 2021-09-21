set(ALL_FILE
    GraphicsManager.hpp
    D3D12GraphicsManager.hpp
    D3D12GraphicsManager.cpp
)

add_library(GraphicsManager ${ALL_FILE})

include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${3RDPARTY_DIR}/d3d12_helper)

target_link_libraries(
    GraphicsManager
    DataType
    D3D12.lib
)
set_target_properties(GraphicsManager PROPERTIES FOLDER "engine/core") 