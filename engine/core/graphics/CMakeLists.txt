set(ALL_FILE
    D3DGraphics/UploadBuffer.hpp
    D3DGraphics/d3dx12.h
    D3DGraphics/d3dUtil.hpp
    D3DGraphics/d3dUtil.cpp
    D3DGraphics/D3D12GraphicsManager.hpp
    D3DGraphics/D3D12GraphicsManager.cpp
    GraphicsManager.hpp
)

add_library(GraphicsManager ${ALL_FILE})

include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)

target_link_libraries(
    GraphicsManager
    DataType
    D3D12.lib
	DXGI.lib
)
set_target_properties(GraphicsManager PROPERTIES FOLDER "engine/core") 