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
        Vector3f Position;
        Vector3f Normal;
        Vector2f TexC;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 3> Desc;
    };

}