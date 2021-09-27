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

        virtual void KeyDown(const uint8_t key) override;
        virtual void KeyUp(const uint8_t key) override;
        virtual bool IsKeyDown(const uint8_t key) const override;

        void SetCurrPosition(const Point2<int16_t>& Position);
        void SetLastPosition(const Point2<int16_t>& Position);
        const Point2<int16_t>& GetLastPosition() const;
        const Point2<int16_t>& GetCurrPosition() const;
        const Point2<int16_t>& GetDeltaPosition() const;
        
    private:
        Point2<int16_t> m_uLastCursorPos; 
        Point2<int16_t> m_uCurrentCursorPos; 

    };

}