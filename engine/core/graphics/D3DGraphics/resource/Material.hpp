#pragma once
#include"Vector.hpp"
#include"Matrix.hpp"
#include<cstdint>
#include<string>

namespace Acorn{

    struct Material{
        Material(std::string name = "") : Name(std::move(name)){}
        std::string Name;

        int16_t MatCBIndex = -1;
        int16_t DiffuseSrvHeapIndex = -1;
        uint16_t NumFramesDirty = 0;

        Acorn::Vector4f DiffuseAlbedo = {1.0f, 1.0f, 1.0f, 1.0f};
        Acorn::Vector3f FresnelR0 = {0.01f, 0.01f, 0.01f};
        float Roughness = 0.25f;
        Acorn::Matrix4f MatTransform = {}; 
    };

}
