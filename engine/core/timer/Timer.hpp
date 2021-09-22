#pragma once
#include"IRuntimeModule.hpp"
#include<cstdint>
#include<windows.h>
namespace AcornEngine{

    class Timer : public IRuntimeModule{
    public:
        Timer();
        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

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