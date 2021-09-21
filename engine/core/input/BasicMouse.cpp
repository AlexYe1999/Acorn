#include"BasicMouse.hpp"

namespace AcornEngine{

    void BasicMouse::SetCurrPosition(const Point2<uint8_t>& Position){
        m_uCurrentCursorPos = Position;
    }

    void BasicMouse::SetLastPosition(const Point2<uint8_t> &Position){
        m_uLastCursorPos = Position;
    }

    inline const Point2<uint8_t>& BasicMouse::GetLastPosition() const {
        return m_uLastCursorPos;
    }
    
    inline const Point2<uint8_t>& BasicMouse::GetCurrPosition() const {
        return m_uCurrentCursorPos;
    }

    inline const Point2<uint8_t>& BasicMouse::GetDeltaPosition() const {
        return m_uCurrentCursorPos - m_uLastCursorPos;
    }

}