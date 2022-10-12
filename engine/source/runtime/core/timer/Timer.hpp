#pragma once
#include<cstdint>
#include<chrono>
#include<windows.h>


namespace Acorn{

    template<typename clock_accuracy>
    struct TimeToSeconds{
        inline static float const value = 1.0f;
    };

    template<>
    struct TimeToSeconds<std::chrono::milliseconds>{
        inline static float const value = 1e-3f;
    };

    template<>
    struct TimeToSeconds<std::chrono::microseconds>{
        inline static float const value = 1e-6f;
    };

    template<>
    struct TimeToSeconds<std::chrono::nanoseconds>{
        inline static float const value = 1e-9f;
    };

    template<typename clock_accuracy>
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

        uint32_t Tick(){
            m_last_tick    = m_current_tick;
            m_current_tick = std::chrono::steady_clock::now();
            return DeltaTime();
        }

        uint32_t TotalTime() const{
            return std::chrono::duration_cast<clock_accuracy>(m_current_tick - m_start_tick).count();
        }

        uint32_t DeltaTime() const{
            return std::chrono::duration_cast<clock_accuracy>(m_current_tick - m_last_tick).count();
        }

        std::chrono::time_point<std::chrono::steady_clock> m_start_tick;
        std::chrono::time_point<std::chrono::steady_clock> m_last_tick;
        std::chrono::time_point<std::chrono::steady_clock> m_current_tick;
        
    };

}