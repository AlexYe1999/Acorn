#include"Vector.hpp"

namespace Acorn{

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
    inline XMVECTOR Vector3f::operator ()() const{
        return XMLoadFloat3(this);
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