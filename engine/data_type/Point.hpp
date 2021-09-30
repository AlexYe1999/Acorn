#pragma once
namespace Acorn{

    template<typename DataType>
    struct Point2{
        union{
            DataType data[2];
            struct{DataType x, y;};
        };
        Point2(DataType _x = 0) : x(_x), y(_x) {}    
        Point2(DataType _x, DataType _y) : x(_x), y(_y) {}
        Point2<DataType> operator +(const Point2<DataType>& pt) const;
        Point2<DataType> operator -(const Point2<DataType>& pt) const;
        Point2<DataType> operator *(DataType factor) const;
        Point2<DataType> operator /(DataType factor) const;
        Point2<DataType>& operator +=(const Point2<DataType>& pt);
        Point2<DataType>& operator -=(const Point2<DataType>& pt);
        Point2<DataType>& operator *=(DataType factor);
        Point2<DataType>& operator /=(DataType factor);

    };

    template<typename DataType>
    inline Point2<DataType> 
    Point2<DataType>::operator +(const Point2<DataType>& pt) const{
        return Point2<DataType>(x+pt.x, y+pt.y);
    }

    template<typename DataType>
    inline Point2<DataType>
    Point2<DataType>::operator -(const Point2<DataType>& pt) const{
        return Point2<DataType>(x-pt.x, y-pt.y);
    }

    template<typename DataType>
    inline Point2<DataType> 
    Point2<DataType>::operator *(DataType factor) const{
        return Point2<DataType>(x*factor, y*factor);
    }

    template<typename DataType>
    inline Point2<DataType> 
    Point2<DataType>::operator /(DataType factor) const{
        return Point2<DataType>(x/factor, y/factor);
    }

    template<typename DataType>
    inline Point2<DataType>&
    Point2<DataType>::operator +=(const Point2<DataType>& pt){
        x += _p.x;
        y += _p.y;
        return *this;
    }
    template<typename DataType>
    inline Point2<DataType>&
    Point2<DataType>::operator -=(const Point2<DataType>& pt){
        x -= _p.x;
        y -= _p.y;
        return *this;
    }

    template<typename DataType>
    inline Point2<DataType>&
    Point2<DataType>::operator *=(DataType factor){
        x *= factor;
        y *= factor;
        return *this;
    }

    template<typename DataType>
    inline Point2<DataType>& 
    Point2<DataType>::operator /=(DataType factor){
        x /= factor;
        y /= factor;
        return *this;
    }

}