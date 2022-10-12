#pragma once

#include "runtime/function/context/engine_context.hpp"
#include "runtime/core/timer/Timer.hpp"

#if defined(__linux__)
    #include<unistd.h>
#elif defined(_WIN32)
    #undef max
    #undef min
    #include<windows.h>
#endif

#include <string>
#include <chrono>
#include <thread>
#include <algorithm>

namespace Acorn{

    class Engine{
    public:
        Engine(Engine const&) = delete;
        Engine const& operator=(Engine&&) = delete;
        Engine const& operator=(Engine const&) = delete;

        static Engine& GetInstance(){
            static Engine s_instance;
            return s_instance;
        }

        void Initialize(){}
        void Clear(){}

        template<typename RuntimeContext = EngineRuntimeContext>
        void StartEngine(){

            m_engine_timer.Initialize();

            m_runtime_context = std::make_unique<RuntimeContext>();
            m_runtime_context->StartSystems();

            m_min_time_per_frame = 1000 / 60;

            m_engine_timer.Tick();

        }

        void ShutdownEngine(){

            m_engine_timer.Tick();

            m_runtime_context->ShutdownSystems();

            m_engine_timer.Tick();
        }

        void Run(){
            while(!m_runtime_context->window_system->ShouldClose()){
                TickOneFrame(m_engine_timer.Tick());
            }
        }

        void TickOneFrame(uint32_t cost_time){

            m_runtime_context->window_system->ProcessMessage();

#if defined(__linux__)
            sleep(std::min(static_cast<uint32_t>(0), static_cast<uint32_t>(m_min_time_per_frame-cost_time)));
#elif defined(_WIN32)
            Sleep(std::min(static_cast<uint32_t>(0), static_cast<uint32_t>(m_min_time_per_frame-cost_time)));
#endif

            static float average_delta_time { 0.0f };
            float delta_time = std::max(cost_time, m_min_time_per_frame) * TimeToSeconds<std::chrono::milliseconds>::value;
            average_delta_time = average_delta_time * 0.8f + delta_time * 0.2f;

            LogicalTick(delta_time);
            RendererTick(delta_time);

            std::string runtime_info =
                "fps : "      + std::to_string(static_cast<uint16_t>(1.0f / average_delta_time)) + " " +
                "cpu time : " + std::to_string(cost_time);

            m_runtime_context->window_system->SetTitle(runtime_info.c_str());

        }

        EngineRuntimeContext* GetEngineRuntimeContext() const{
            return m_runtime_context.get();
        }

    protected:

        void LogicalTick(float delta_time){

        }

        void RendererTick(float delta_time){

        }

    protected:
        Engine() = default;
        virtual ~Engine() = default;

        uint32_t                              m_min_time_per_frame = 0;
        Timer<std::chrono::milliseconds>      m_engine_timer;
        std::unique_ptr<EngineRuntimeContext> m_runtime_context;

    };


}