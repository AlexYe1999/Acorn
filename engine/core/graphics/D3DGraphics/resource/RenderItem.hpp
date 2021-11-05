#pragma once
#include<cstdint>
#include<D3D12.h>
#include"Matrix.hpp"
#include"Material.hpp"
#include"Mesh.hpp"

namespace Acorn{

    enum class RenderLayer : uint16_t {
        Opaque = 0,
        Mirrors,
        Reflected,
        Transparent,
        Shadow,
        Sprite,
        LayerCount
    };

    struct InstanceData{
        Matrix4f World;
        Matrix4f WorldIT;
        Matrix4f TexTransform;
        uint32_t MaterialIndex;
        Vector3f Pad;
    };

    struct RenderItem{
        RenderItem() = default;
        RenderItem(const RenderItem& rhs) = delete;
        
        Matrix4f World;
        uint32_t ObjCBIndex = 0;

        Mesh* Mesh = nullptr;
        Material* Mat = nullptr;
        D3D12_PRIMITIVE_TOPOLOGY PrimitiveType 
            = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        uint16_t DirtyCount = 0;
        uint16_t IndexCount = 0;
        uint32_t StartIndexLocation  = 0;
        uint32_t StartVertexLocation = 0;

    };

    struct RenderInstance{
        RenderInstance() = default;
        RenderInstance(const RenderInstance& rhs) = delete;
        
        Mesh* Mesh = nullptr;
        SubMesh* SubMesh = nullptr;
        std::vector<InstanceData> InstanceDataArray;
        D3D12_PRIMITIVE_TOPOLOGY PrimitiveType
            = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


        uint16_t DirtyCount = 0;
        uint16_t IndexCount = 0;
        uint32_t StartIndexLocation  = 0;
        uint32_t StartVertexLocation = 0;
        uint32_t VisibleInstanceCount = 0;

    };
}