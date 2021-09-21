#pragma once
#include"IKeyModule.hpp"
#include"Point.hpp"

namespace AcornEngine{

    class BasicMouse : public IKeyModule<3>{
    public:
        BasicMouse() : IKeyModule<3>(), m_uLastCursorPos(), m_uCurrentCursorPos(){}
        virtual void Initialize() override {}
        virtual void Finalize() override {}
        virtual void Tick() override {}

        void SetCurrPosition(const Point2<uint8_t>& Position);
        void SetLastPosition(const Point2<uint8_t>& Position);
        const Point2<uint8_t>& GetLastPosition() const;
        const Point2<uint8_t>& GetCurrPosition() const;
        const Point2<uint8_t>& GetDeltaPosition() const;
        
    private:
        Point2<uint8_t> m_uLastCursorPos; 
        Point2<uint8_t> m_uCurrentCursorPos; 

    };

}