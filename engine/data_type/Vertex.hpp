#include"Vector.hpp"
#include<D3D12.h>
#include<array>

namespace Acorn{

    struct VertexTest{
        Vector3f Position;
        Vector4f Color;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 2> Desc;
    };


}