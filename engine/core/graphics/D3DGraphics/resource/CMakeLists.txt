include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${ENGINE_DIR}/core/camera)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(Geometry
    Mesh.hpp
    Mesh.cpp
    Scene.hpp
    Scene.cpp
    RenderItem.hpp
    RenderItem.cpp
    CubeRenderTarget.h
    CubeRenderTarget.cpp
    GeometryGenerator.hpp
    GeometryGenerator.cpp    
)

set(Frame_Resource
    Light.hpp
    Texture.hpp
    Material.hpp
    Constant.hpp
    UploadBuffer.hpp
    FrameResource.hpp
    FrameResource.cpp
)

add_library(D3DResource ${Geometry} ${Frame_Resource})
target_link_libraries(D3DResource
    Camera
    DataType
    D3D12.lib
    DXGI.lib
)
set_target_properties(D3DResource PROPERTIES FOLDER "engine/core") 