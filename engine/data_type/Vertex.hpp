#pragma once
#include"Vector.hpp"
#include<D3D12.h>
#include<array>

namespace Acorn{

    struct VertexP3C4{
        Vector3f Position;
        Vector4f Color;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 2> Desc;
    };

    struct TreeSpriteVertex{
        TreeSpriteVertex() = default;
        TreeSpriteVertex(
            float x, float y, float z,
            float width, float height
        ) : 
            Position(x, y, z),
            Size(width, height)
        {}
        Vector3f Position;
        Vector2f Size;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 2> Desc;
    };

    struct Vertex{
        Vertex() = default;
        Vertex(
            float x, float y, float z, 
            float nx, float ny, float nz,
            float u, float v
        ) :
            Position(x, y, z),
            Normal(nx, ny, nz),
            TexC(u, v)
        {}
        Vertex(
            const Vector3f& position, 
            const Vector3f& normal,
            const Vector2f& TexC
        ) :
            Position(position),
            Normal(normal),
            TexC(TexC)
        {}
        Vertex(
            const XMFLOAT3& position, 
            const XMFLOAT3& normal,
            const XMFLOAT2& TexC
        ) :
            Position(position),
            Normal(normal),
            TexC(TexC)
        {}
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexC;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 3> Desc;
    };

}