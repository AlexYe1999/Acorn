#include"BasicMouse.hpp"

namespace AcornEngine{

    void BasicMouse::KeyDown(const uint8_t key){
        m_bKeyBitSet[key] = true;
    }

    void BasicMouse::KeyUp(const uint8_t key){
        m_bKeyBitSet[key] = false;
    }

    bool BasicMouse::IsKeyDown(const uint8_t key) const{
        return m_bKeyBitSet[key];
    }

    void BasicMouse::SetCurrPosition(const Point2<int16_t>& Position){
        m_uCurrentCursorPos = Position;
    }

    void BasicMouse::SetLastPosition(const Point2<int16_t>& Position){
        m_uLastCursorPos = Position;
    }

    const Point2<int16_t>& BasicMouse::GetLastPosition() const {
        return m_uLastCursorPos;
    }
    
    const Point2<int16_t>& BasicMouse::GetCurrPosition() const {
        return m_uCurrentCursorPos;
    }

    const Point2<int16_t>& BasicMouse::GetDeltaPosition() const {
        return m_bKeyBitSet[0] == true ? m_uCurrentCursorPos - m_uLastCursorPos : 0;
    }

}