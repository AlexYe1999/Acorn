add_subdirectory(D3DGraphics/resource)
include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${ENGINE_DIR}/core/timer)
include_directories(${ENGINE_DIR}/core/camera)
include_directories(${ENGINE_DIR}/core/graphics/D3DGraphics/resource)
#include_directories(${3RDPARTY_DIR}/DirectXTK12/Inc)

set(ALL_FILE
    D3DGraphics/d3dx12.h
    D3DGraphics/d3dUtil.hpp
    D3DGraphics/d3dUtil.cpp
    D3DGraphics/D3D12GraphicsManager.hpp
    D3DGraphics/D3D12GraphicsManager.cpp
    D3DGraphics/DDSTextureLoader.h
    D3DGraphics/DDSTextureLoader.cpp
    GraphicsManager.hpp
)

add_library(GraphicsManager ${ALL_FILE})

target_link_libraries(
    GraphicsManager
    Timer
    DataType
    D3DResource
)

target_link_libraries(
    GraphicsManager
    D3D12.lib
	DXGI.lib
)

set_target_properties(GraphicsManager PROPERTIES FOLDER "engine/core") 