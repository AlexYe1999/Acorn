#pragma once
#include<DirectXPackedVector.h>

namespace AcornEngine{
    using namespace DirectX::PackedVector;

    class Color : XMCOLOR{
        Color() : XMCOLOR(){}
        Color(uint32_t color) : XMCOLOR(color){}
        Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
            : XMCOLOR(a, r, g, b){}
    };


}