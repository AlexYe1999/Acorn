include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
include_directories(${ENGINE_DIR}/core/camera)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(ALL_FILE
    Mesh.hpp
    Mesh.cpp
    Scene.hpp
    Scene.cpp
    RenderItem.hpp
    RenderItem.cpp
    GeometryGenerator.hpp
    GeometryGenerator.cpp    
    FrameResource.hpp
    FrameResource.cpp
)

add_library(D3DResource ${ALL_FILE})
target_link_libraries(D3DResource
    Camera
    DataType
    D3D12.lib
    DXGI.lib
)
set_target_properties(D3DResource PROPERTIES FOLDER "engine/core") 