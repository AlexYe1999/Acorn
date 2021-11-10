#pragma once
#include<DirectXMath.h>

namespace Acorn{
    using namespace DirectX;

    struct Vector2f : DirectX::XMFLOAT2{
        Vector2f() = default;
        Vector2f(const XMFLOAT2& vec) : XMFLOAT2(vec) {}
        Vector2f(const float x) : XMFLOAT2(x, x) {}
        Vector2f(const float x, const float y) : XMFLOAT2(x, y) {}

        Vector2f operator +(const Vector2f& vec) const;
        Vector2f operator -(const Vector2f& vec) const;
        Vector2f operator *(const Vector2f& vec) const;
        Vector2f operator /(const Vector2f& vec) const;
        Vector2f operator *(const float factor) const;
        Vector2f operator /(const float factor) const;
        Vector2f& operator =(const XMFLOAT2& vec);
        Vector2f& operator +=(const Vector2f& vec);
        Vector2f& operator -=(const Vector2f& vec);
        Vector2f& operator *=(const Vector2f& vec);
        Vector2f& operator /=(const Vector2f& vec);
        Vector2f& operator *=(const float factor);
        Vector2f& operator /=(const float factor);
    };

    struct Vector3f : DirectX::XMFLOAT3{
        Vector3f() = default;
        Vector3f(const XMFLOAT3& vec) : XMFLOAT3(vec){}
        Vector3f(const float x) : XMFLOAT3(x, x, x){}
        Vector3f(const float x, const float y, const float z) : DirectX::XMFLOAT3(x, y, z) {}

        Vector3f operator +(const Vector3f& vec) const;
        Vector3f operator -(const Vector3f& vec) const;
        Vector3f operator *(const Vector3f& vec) const;
        Vector3f operator /(const Vector3f& vec) const;
        Vector3f operator *(const float factor) const;
        Vector3f operator /(const float factor) const;
        Vector3f& operator =(const XMVECTOR& vec);
        Vector3f& operator +=(const Vector3f& vec);
        Vector3f& operator -=(const Vector3f& vec);
        Vector3f& operator *=(const Vector3f& vec);
        Vector3f& operator /=(const Vector3f& vec);
        Vector3f& operator *=(const float factor);
        Vector3f& operator /=(const float factor);
    };

    struct Vector4f : DirectX::XMFLOAT4{
        Vector4f() = default;
        Vector4f(const XMVECTOR& vector){ XMStoreFloat4(this, vector); }
        Vector4f(const float x) : XMFLOAT4(x, x, x, x){}
        Vector4f(const float x, const float y, const float z, const float w)
            : XMFLOAT4(x, y, z, w){}

        Vector4f operator +(const Vector4f& Vector4) const;
        Vector4f operator -(const Vector4f& Vector4) const;
        Vector4f operator *(const Vector4f& DataType) const;
        Vector4f operator /(const Vector4f& DataType) const;
        Vector4f operator *(const float factor) const;
        Vector4f operator /(const float factor) const;
        XMVECTOR operator ()() const;
        Vector4f& operator +=(const Vector4f& Vector4);
        Vector4f& operator -=(const Vector4f& Vector4);
        Vector4f& operator *=(const Vector4f& DataType);
        Vector4f& operator /=(const Vector4f& DataType);
        Vector4f& operator *=(const float factor);
        Vector4f& operator /=(const float factor);
    };

    inline Vector2f Vector2f::operator +(const Vector2f& vec) const{
        Vector2f vector;
        XMStoreFloat2(&vector, XMLoadFloat2(this)+XMLoadFloat2(&vec));
        return vector;
    }
    inline Vector2f Vector2f::operator -(const Vector2f& vec) const{
        Vector2f vector;
        XMStoreFloat2(&vector, XMLoadFloat2(this)-XMLoadFloat2(&vec));
        return vector;
    }
    inline Vector2f Vector2f::operator *(const Vector2f& vec) const{
        Vector2f vector;
        XMStoreFloat2(&vector, XMLoadFloat2(this)*XMLoadFloat2(&vec));
        return vector;
    }
    inline Vector2f Vector2f::operator /(const Vector2f& vec) const{
        Vector2f vector;
        XMStoreFloat2(&vector, XMLoadFloat2(this)/XMLoadFloat2(&vec));
        return vector;
    }
    inline Vector2f Vector2f::operator *(const float factor) const{
        Vector2f vector(factor);
        XMStoreFloat2(&vector, XMLoadFloat2(this)*XMLoadFloat2(&vector));
        return vector;
    }
    inline Vector2f Vector2f::operator /(const float factor) const{
        Vector2f vector(factor);
        XMStoreFloat2(&vector, XMLoadFloat2(this)/XMLoadFloat2(&vector));
        return vector;
    }
    inline Vector2f& Vector2f::operator =(const XMFLOAT2& vec){
        x = vec.x;
        y = vec.y;
        return *this;
    }
    inline Vector2f& Vector2f::operator +=(const Vector2f& vec){
        XMStoreFloat2(this, XMLoadFloat2(this)+XMLoadFloat2(&vec));
        return *this;
    }
    inline Vector2f& Vector2f::operator -=(const Vector2f& vec){
        XMStoreFloat2(this, XMLoadFloat2(this)-XMLoadFloat2(&vec));
        return *this;
    }
    inline Vector2f& Vector2f::operator *=(const Vector2f& vec){
        XMStoreFloat2(this, XMLoadFloat2(this)*XMLoadFloat2(&vec));
        return *this;
    }
    inline Vector2f& Vector2f::operator /=(const Vector2f& vec){
        XMStoreFloat2(this, XMLoadFloat2(this)/XMLoadFloat2(&vec));
        return *this;
    }
    inline Vector2f& Vector2f::operator *=(const float factor){
        XMStoreFloat2(this, XMLoadFloat2(this)*XMLoadFloat2(&Vector2f(factor)));
        return *this;
    }
    inline Vector2f& Vector2f::operator /=(const float factor){
        XMStoreFloat2(this, XMLoadFloat2(this)/XMLoadFloat2(&Vector2f(factor)));
        return *this;
    }

    inline Vector3f Vector3f::operator +(const Vector3f& vec) const{
        Vector3f vector;
        XMStoreFloat3(&vector, XMLoadFloat3(this)+XMLoadFloat3(&vec));
        return vector;
    }
    inline Vector3f Vector3f::operator -(const Vector3f& vec) const{
        Vector3f vector;
        XMStoreFloat3(&vector, XMLoadFloat3(this)-XMLoadFloat3(&vec));
        return vector;
    }
    inline Vector3f Vector3f::operator *(const Vector3f& vec) const{
        Vector3f vector;
        XMStoreFloat3(&vector, XMLoadFloat3(this)*XMLoadFloat3(&vec));
        return vector;
    }
    inline Vector3f Vector3f::operator /(const Vector3f& vec) const{
        Vector3f vector;
        XMStoreFloat3(&vector, XMLoadFloat3(this)/XMLoadFloat3(&vec));
        return vector;
    }
    inline Vector3f Vector3f::operator *(const float factor) const{
        Vector3f vector(factor);
        XMStoreFloat3(&vector, XMLoadFloat3(this)*XMLoadFloat3(&vector));
        return vector;
    }
    inline Vector3f Vector3f::operator /(const float factor) const{
        Vector3f vector(factor);
        XMStoreFloat3(&vector, XMLoadFloat3(this)/XMLoadFloat3(&vector));
        return vector;
    }
    inline Vector3f& Vector3f::operator =(const XMVECTOR& vec){
        XMStoreFloat3(this, vec);
        return *this;
    }
    inline Vector3f& Vector3f::operator +=(const Vector3f& vec){
        XMStoreFloat3(this, XMLoadFloat3(this)+XMLoadFloat3(&vec));
        return *this;
    }
    inline Vector3f& Vector3f::operator -=(const Vector3f& vec){
        XMStoreFloat3(this, XMLoadFloat3(this)-XMLoadFloat3(&vec));
        return *this;
    }
    inline Vector3f& Vector3f::operator *=(const Vector3f& vec){
        XMStoreFloat3(this, XMLoadFloat3(this)*XMLoadFloat3(&vec));
        return *this;
    }
    inline Vector3f& Vector3f::operator /=(const Vector3f& vec){
        XMStoreFloat3(this, XMLoadFloat3(this)/XMLoadFloat3(&vec));
        return *this;
    }
    inline Vector3f& Vector3f::operator *=(const float factor){
        Vector3f vector(factor);
        XMStoreFloat3(this, XMLoadFloat3(this)*XMLoadFloat3(&vector));
        return *this;
    }
    inline Vector3f& Vector3f::operator /=(const float factor){
        Vector3f vector(factor);
        XMStoreFloat3(this, XMLoadFloat3(this)/XMLoadFloat3(&vector));
        return *this;
    }

    inline Vector4f Vector4f::operator +(const Vector4f& vec) const{
        Vector4f vector;
        XMStoreFloat4(&vector, XMLoadFloat4(this)+XMLoadFloat4(&vec));
        return vector;
    }
    inline Vector4f Vector4f::operator -(const Vector4f& vec) const{
        Vector4f vector;
        XMStoreFloat4(&vector, XMLoadFloat4(this)-XMLoadFloat4(&vec));
        return vector;
    }
    inline Vector4f Vector4f::operator *(const Vector4f& vec) const{
        Vector4f vector;
        XMStoreFloat4(&vector, XMLoadFloat4(this)*XMLoadFloat4(&vec));
        return vector;
    }
    inline Vector4f Vector4f::operator /(const Vector4f& vec) const{
        Vector4f vector;
        XMStoreFloat4(&vector, XMLoadFloat4(this)/XMLoadFloat4(&vec));
        return vector;
    }
    inline Vector4f Vector4f::operator *(const float factor) const{
        Vector4f vector(factor);
        XMStoreFloat4(&vector, XMLoadFloat4(this)*XMLoadFloat4(&vector));
        return vector;
    }
    inline Vector4f Vector4f::operator /(const float factor) const{
        Vector4f vector(factor);
        XMStoreFloat4(&vector, XMLoadFloat4(this)/XMLoadFloat4(&vector));
        return vector;
    }
    inline Vector4f& Vector4f::operator +=(const Vector4f& vec){
        XMStoreFloat4(this, XMLoadFloat4(this)+XMLoadFloat4(&vec));
        return *this;
    }
    inline Vector4f& Vector4f::operator -=(const Vector4f& vec){
        XMStoreFloat4(this, XMLoadFloat4(this)-XMLoadFloat4(&vec));
        return *this;
    }
    inline Vector4f& Vector4f::operator *=(const Vector4f& vec){
        XMStoreFloat4(this, XMLoadFloat4(this)*XMLoadFloat4(&vec));
        return *this;
    }
    inline Vector4f& Vector4f::operator /=(const Vector4f& vec){
        XMStoreFloat4(this, XMLoadFloat4(this)/XMLoadFloat4(&vec));
        return *this;
    }
    inline Vector4f& Vector4f::operator *=(const float factor){
        Vector4f vector(factor);
        XMStoreFloat4(this, XMLoadFloat4(this)*XMLoadFloat4(&vector));
        return *this;
    }
    inline Vector4f& Vector4f::operator /=(const float factor){
        Vector4f vector(factor);
        XMStoreFloat4(this, XMLoadFloat4(this)/XMLoadFloat4(&vector));
        return *this;
    }

}