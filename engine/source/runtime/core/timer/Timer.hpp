#pragma once
#include<cstdint>
#include<windows.h>
namespace Acorn{

    class Timer{
    public:
        Timer();
        virtual void Initialize() ;
        virtual void Finalize() ;
        virtual void Tick() ;

        void Reset();
        void Stop();

        float TotalTime() const;
        float DeltaTime() const;

    private:
        bool m_bIsStopped;
        double m_SecondPerCount;

        uint64_t m_uBeginCount;
        uint64_t m_uCurrCount;
        uint64_t m_uPreCount;
        uint64_t m_uStopTime;
        uint64_t m_uPausedTime;
    };

}