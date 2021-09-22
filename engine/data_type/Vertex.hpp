#include"Vector.hpp"
#include<D3D12.h>
namespace AcornEngine{

    struct VectexTest{
        Vector3f Position;
        Vector3f Color;
        static D3D12_INPUT_ELEMENT_DESC Desc[];
    };
    
    D3D12_INPUT_ELEMENT_DESC VectexTest::Desc[] = {
        {"Position", 0, DXGI_FORMAT_R32G32B32FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"Color",    0, DXGI_FORMAT_R32G32B32FLOAT, 12, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };



}