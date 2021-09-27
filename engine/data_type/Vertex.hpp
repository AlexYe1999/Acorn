#include"Vector.hpp"
#include<D3D12.h>
#include<array>

namespace AcornEngine{

    struct VertexTest{
        Vector3f Position;
        Vector4f Color;
        static std::array<D3D12_INPUT_ELEMENT_DESC, 2> Desc;
    };


}