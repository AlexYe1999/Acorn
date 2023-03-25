#pragma once

#include<ratio>
#include<chrono>
#include<cstdint>

namespace Acorn{

    template<typename rep, typename period>
    class Timer{
    public:
        Timer() 
            : m_start_tick(std::chrono::steady_clock::now())
            , m_last_tick(std::chrono::steady_clock::now())
            , m_current_tick(std::chrono::steady_clock::now())
        {}

        void Initialize(){
            Reset();
        }

        void Reset(){
            m_start_tick   = std::chrono::steady_clock::now();
            m_last_tick    = m_start_tick;
            m_current_tick = m_start_tick;
        }

        void Tick(){
            m_last_tick    = m_current_tick;
            m_current_tick = std::chrono::steady_clock::now();
        }

        std::chrono::duration<rep, period> ElapsedTime() const{
            return std::chrono::duration_cast<std::chrono::duration<rep, period>>(std::chrono::steady_clock::now() - m_current_tick);
        }

        std::chrono::duration<rep, period> DeltaTime() const{
            return std::chrono::duration_cast<std::chrono::duration<rep, period>>(m_current_tick - m_last_tick);
        }

        std::chrono::duration<rep, period> TotalTime() const {
            return std::chrono::duration_cast<std::chrono::duration<rep, period>>(m_current_tick - m_start_tick);
        }

        std::chrono::time_point<std::chrono::steady_clock> m_start_tick;
        std::chrono::time_point<std::chrono::steady_clock> m_last_tick;
        std::chrono::time_point<std::chrono::steady_clock> m_current_tick;
        
    };

}