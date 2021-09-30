#include"Timer.hpp"
namespace Acorn{

    Timer::Timer()
        :
        m_bIsStopped(false),
        m_SecondPerCount(1),
        m_uBeginCount(0), 
        m_uCurrCount(0),
        m_uPreCount(0),
        m_uStopTime(0),
        m_uPausedTime(0)
    {
        uint64_t count;
        QueryPerformanceFrequency((LARGE_INTEGER*)&count);
        m_SecondPerCount = 1.0 / count;
    }

    void Timer::Initialize(){
        uint64_t count;
        QueryPerformanceFrequency((LARGE_INTEGER*)&count);
        m_SecondPerCount = 1.0 / count;
    }

    void Timer::Finalize(){

    }

    void Timer::Tick(){
        if(m_bIsStopped) return;
        m_uPreCount = m_uCurrCount;
        QueryPerformanceCounter((LARGE_INTEGER*)&m_uCurrCount);
    }

    void Timer::Reset(){
        m_bIsStopped = false;
        QueryPerformanceCounter((LARGE_INTEGER*)&m_uBeginCount);
        m_uCurrCount  = m_uBeginCount; 
        m_uPreCount   = m_uBeginCount; 
        m_uStopTime   = m_uBeginCount; 
        m_uPausedTime = 0; 
    }

    void Timer::Stop(){
        if(!m_bIsStopped){
            QueryPerformanceCounter((LARGE_INTEGER*)&m_uStopTime);
            m_bIsStopped = true;
        }
    }

    float Timer::TotalTime() const{
        uint64_t currTime;
        uint64_t totalCount = 0;

        QueryPerformanceCounter((LARGE_INTEGER*)&currTime); 
        
        totalCount = currTime - m_uBeginCount - m_uPausedTime;
        if(m_bIsStopped){
            totalCount -= currTime - m_uStopTime;
        }
        return static_cast<float>(totalCount * m_SecondPerCount);
    }

    float Timer::DeltaTime() const{
        return static_cast<float>((m_uCurrCount-m_uPreCount) * m_SecondPerCount);
    }

}