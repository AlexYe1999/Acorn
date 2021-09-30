#pragma once
#include<DirectXMath.h>

namespace Acorn{
    using namespace DirectX;

    struct Matrix4f : XMFLOAT4X4{
        Matrix4f() : XMFLOAT4X4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ){}
        Matrix4f(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        ) : XMFLOAT4X4(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33)
        {}
        Matrix4f(const XMMATRIX& matrix) : XMFLOAT4X4(){
            XMStoreFloat4x4(this, matrix);
        }
        explicit Matrix4f(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray){}
        void operator=(const XMMATRIX& matrix){
            XMStoreFloat4x4(this, matrix);
        }
        XMMATRIX operator()(const XMMATRIX& matrix){
            return XMLoadFloat4x4(this);
        }
    };

}