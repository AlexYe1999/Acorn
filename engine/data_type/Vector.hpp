#pragma once
#include<DirectXMath.h>

namespace Acorn{
    using namespace DirectX;

    struct Vector2f : DirectX::XMFLOAT2{
        Vector2f() : XMFLOAT2(1.0f, 1.0f){}
        Vector2f(const float x) : XMFLOAT2(x, x) {}

        Vector2f operator +(const Vector2f& vec) const;
        Vector2f operator -(const Vector2f& vec) const;
        Vector2f operator *(const Vector2f& vec) const;
        Vector2f operator /(const Vector2f& vec) const;
        Vector2f operator *(const float factor) const;
        Vector2f operator /(const float factor) const;
        Vector2f& operator +=(const Vector2f& vec);
        Vector2f& operator -=(const Vector2f& vec);
        Vector2f& operator *=(const Vector2f& vec);
        Vector2f& operator /=(const Vector2f& vec);
        Vector2f& operator *=(const float factor);
        Vector2f& operator /=(const float factor);
    };

    struct Vector3f : DirectX::XMFLOAT3{
        Vector3f() : XMFLOAT3(1.0f, 1.0f, 1.0f){}
        Vector3f(const float x) : XMFLOAT3(x, x, x){}
        Vector3f(const float x, const float y, const float z) : DirectX::XMFLOAT3(x, y, z) {}

        Vector3f operator +(const Vector3f& vec) const;
        Vector3f operator -(const Vector3f& vec) const;
        Vector3f operator *(const Vector3f& vec) const;
        Vector3f operator /(const Vector3f& vec) const;
        Vector3f operator *(const float factor) const;
        Vector3f operator /(const float factor) const;
        XMVECTOR operator ()() const;
        Vector3f& operator =(const XMVECTOR& vec);
        Vector3f& operator +=(const Vector3f& vec);
        Vector3f& operator -=(const Vector3f& vec);
        Vector3f& operator *=(const Vector3f& vec);
        Vector3f& operator /=(const Vector3f& vec);
        Vector3f& operator *=(const float factor);
        Vector3f& operator /=(const float factor);
    };

    struct Vector4f : DirectX::XMFLOAT4{
        Vector4f(const XMVECTOR& vector){ XMStoreFloat4(this, vector); }
        Vector4f() : XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f){}
        Vector4f(const float x) : XMFLOAT4(x, x, x, x){}
        Vector4f(const float x, const float y, const float z, const float w)
            : XMFLOAT4(x, y, z, w){}

        Vector4f operator +(const Vector4f& Vector4) const;
        Vector4f operator -(const Vector4f& Vector4) const;
        Vector4f operator *(const Vector4f& DataType) const;
        Vector4f operator /(const Vector4f& DataType) const;
        Vector4f operator *(const float factor) const;
        Vector4f operator /(const float factor) const;
        Vector4f& operator +=(const Vector4f& Vector4);
        Vector4f& operator -=(const Vector4f& Vector4);
        Vector4f& operator *=(const Vector4f& DataType);
        Vector4f& operator /=(const Vector4f& DataType);
        Vector4f& operator *=(const float factor);
        Vector4f& operator /=(const float factor);
    };

}