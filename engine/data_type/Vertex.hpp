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

    struct VertexPNC{
        Vector3f Position;
        Vector3f Normal;
        Vector4f Color;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 3> Desc;
    };

}