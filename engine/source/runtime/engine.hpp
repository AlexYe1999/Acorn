#pragma once

#include "runtime/function/context/engine_context.hpp"
#include "runtime/core/timer/Timer.hpp"

#include <string>
#include <ratio>
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

            m_time_per_frame = 1000.0f / 60.0f;

            m_engine_timer.Tick();

        }

        void ShutdownEngine(){

            m_engine_timer.Tick();

            m_runtime_context->ShutdownSystems();

            m_engine_timer.Tick();
        }

        void Run(){
            while(!m_runtime_context->window_system->ShouldClose()){
                TickOneFrame();
            }
        }

        void TickOneFrame(){

            float delta_time = m_engine_timer.DeltaTime().count();

            static float average_delta_time { 0.0f };
            average_delta_time = average_delta_time * 0.8f + delta_time * 0.2f;

            m_engine_timer.Tick();

            LogicalTick(delta_time);

            float cpu_costs = m_engine_timer.ElapsedTime().count();

            RenderTick(delta_time);

            float render_costs = m_engine_timer.ElapsedTime().count() - cpu_costs;

            while (m_engine_timer.ElapsedTime().count() < m_time_per_frame) {
                m_runtime_context->window_system->ProcessMessage();
            }

            std::string runtime_info = 
                "fps : " + std::to_string(1000.0f / average_delta_time) + " " +
                "cpu time : " + std::to_string(cpu_costs) + " ms" + " " + 
                "render time ; " + std::to_string(render_costs) + "ms";

            m_runtime_context->window_system->SetTitle(runtime_info.c_str());

        }

        EngineRuntimeContext* GetEngineRuntimeContext() const{
            return m_runtime_context.get();
        }

    protected:

        void LogicalTick(float delta_time){

        }

        void RenderTick(float delta_time){

        }

    protected:
        Engine() = default;
        virtual ~Engine() = default;

        float                                   m_time_per_frame { 0.0f };
        Timer<float, std::milli>                m_engine_timer;
        std::unique_ptr<EngineRuntimeContext>   m_runtime_context;

    };


}