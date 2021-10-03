#pragma once
#include<cstdint>
#include<D3D12.h>
#include"Matrix.hpp"
#include"Mesh.hpp"

namespace Acorn{
    struct RenderItem{
        RenderItem() = default;

        Matrix4f World;
        uint32_t ObjCBIndex = 0;

        Mesh* Mesh = nullptr;
        D3D12_PRIMITIVE_TOPOLOGY PrimitiveType 
            = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        uint16_t DirtyCount = 0;
        uint16_t IndexCount = 0;
        uint32_t StartIndexLocation  = 0;
        uint32_t StartVertexLocation = 0;

    };
}