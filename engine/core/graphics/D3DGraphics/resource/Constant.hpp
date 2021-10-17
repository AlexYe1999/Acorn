#pragma once
#include"Vector.hpp"
#include"Matrix.hpp"
#include"Light.hpp"
namespace Acorn{
    struct PassConstant{
        Matrix4f View;
        Matrix4f InvView;
        Matrix4f Proj;
        Matrix4f InvProj;
        Matrix4f ViewProj;
        Matrix4f InvViewProj;
        Vector3f EyePosW;
        float pad1;
        Vector2f RenderTargetSize;
        Vector2f InvRenderTargetSize;
        float NearZ;
        float FarZ;
        float TotalTime;
        float DeltaTime;
        Vector4f AmbientLight;
        Light Lights[3];
        PassConstant() = default;
    }; //size 6*16 + 4 + 4 + 4 = 108*4 = 432

    struct ObjectConstant{
        Matrix4f World;
        Matrix4f WorldIT;
        ObjectConstant() = default;
    }; // size 16*4 = 64

    struct MaterialConstant{
        Vector4f DiffuseAlbedo = {1.0f, 1.0f, 1.0f, 1.0f};
        Vector3f FresnelR0 = {0.01f, 0.01f, 0.01f};
        float Roughness = 0.25f;
        Matrix4f MatTransform = {}; 
    }; // size 4 + 4 + 16 = 24 * 4 = 96
}