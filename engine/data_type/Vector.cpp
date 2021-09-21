#include"Vector.hpp"

namespace AcornEngine{
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

    }
    inline Vector2f Vector2f::operator /(const Vector2f& vec) const{

    }
    inline Vector2f Vector2f::operator *(const float factor) const{

    }
    inline Vector2f Vector2f::operator /(const float factor) const{

    }
    inline Vector2f& Vector2f::operator +=(const Vector2f& vec){

    }
    inline Vector2f& Vector2f::operator -=(const Vector2f& vec){

    }
    inline Vector2f& Vector2f::operator *=(const Vector2f& vec){

    }
    inline Vector2f& Vector2f::operator /=(const Vector2f& vec){

    }
    inline Vector2f& Vector2f::operator *=(const float factor){

    }
    inline Vector2f& Vector2f::operator /=(const float factor){

    }


    inline Vector3f Vector3f::operator +(const Vector3f& vec) const{

    }
    inline Vector3f Vector3f::operator -(const Vector3f& vec) const{

    }
    inline Vector3f Vector3f::operator *(const Vector3f& vec) const{

    }
    inline Vector3f Vector3f::operator /(const Vector3f& vec) const{

    }
    inline Vector3f Vector3f::operator *(const float factor) const{

    }
    inline Vector3f Vector3f::operator /(const float factor) const{

    }
    inline Vector3f& Vector3f::operator +=(const Vector3f& vec){

    }
    inline Vector3f& Vector3f::operator -=(const Vector3f& vec){

    }
    inline Vector3f& Vector3f::operator *=(const Vector3f& vec){

    }
    inline Vector3f& Vector3f::operator /=(const Vector3f& vec){

    }
    inline Vector3f& Vector3f::operator *=(const float factor){

    }
    inline Vector3f& Vector3f::operator /=(const float factor){

    }


    inline Vector4f Vector4f::operator +(const Vector4f& vec) const{

    }
    inline Vector4f Vector4f::operator -(const Vector4f& vec) const{

    }
    inline Vector4f Vector4f::operator *(const Vector4f& vec) const{

    }
    inline Vector4f Vector4f::operator /(const Vector4f& vec) const{

    }
    inline Vector4f Vector4f::operator *(const float factor) const{

    }
    inline Vector4f Vector4f::operator /(const float factor) const{

    }
    inline Vector4f& Vector4f::operator +=(const Vector4f& vec){

    }
    inline Vector4f& Vector4f::operator -=(const Vector4f& vec){

    }
    inline Vector4f& Vector4f::operator *=(const Vector4f& vec){

    }
    inline Vector4f& Vector4f::operator /=(const Vector4f& vec){

    }
    inline Vector4f& Vector4f::operator *=(const float factor){

    }
    inline Vector4f& Vector4f::operator /=(const float factor){

    }


}