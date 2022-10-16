#pragma once

#include "runtime/function/context/engine_context.hpp"
#include "runtime/core/timer/Timer.hpp"

#include <cassert>
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

        template<typename RuntimeContext = RuntimeContext>
        void InitEngine(){

            m_engine_timer.InitTimer();

            m_runtime_context = std::make_unique<RuntimeContext>();
            m_runtime_context->InitSystems();
        
        }

        void StartEngine(){

            assert(m_runtime_context);
            m_runtime_context->StartSystems();
        
            float frame_rate = Engine::GetInstance().GetRuntimeContext()->GetConfigSystem()->GetEngineConfig().fps;
            m_time_per_frame = 1000.0f / frame_rate;

            m_engine_timer.Tick();

        }

        void ShutdownEngine(){

            m_engine_timer.Tick();
            
            assert(m_runtime_context);
            m_runtime_context->ShutdownSystems();

            m_engine_timer.Tick();
        }

        void Run(){

            assert(m_runtime_context);
            while(!m_runtime_context->GetWindowSystem()->ShouldClose()){
                TickOneFrame();
            }
        }

        void TickOneFrame(){

            float delta_time = m_engine_timer.DeltaTime().count() * 1e-3f;

            static float average_delta_time { 0.0f };
            average_delta_time = average_delta_time * 0.8f + delta_time * 0.2f;

            m_engine_timer.Tick();

            LogicalTick(delta_time);

            float cpu_costs = m_engine_timer.ElapsedTime().count();

            RendererTick(delta_time);

            float render_costs = m_engine_timer.ElapsedTime().count() - cpu_costs;

            auto window = m_runtime_context->GetWindowSystem();

            while (m_engine_timer.ElapsedTime().count() < m_time_per_frame) {
                window->ProcessMessage();
            }

            std::string runtime_info = 
                "cpu time : " + std::to_string(cpu_costs) + " ms" + " - " + 
                "render time " + std::to_string(render_costs) + " ms" + " - " +
                "fps : " + std::to_string(1.0f / average_delta_time);

            window->SetTitle(runtime_info.c_str());

        }

        RuntimeContext* GetRuntimeContext() const{
            return m_runtime_context.get();
        }

    protected:

        void LogicalTick(float delta_time){

        }

        void RendererTick(float delta_time){

        }

    private:
        Engine() = default;
        virtual ~Engine() = default;

        float                             m_time_per_frame  { 1.0f };
        Timer<float, std::milli>          m_engine_timer;
        std::unique_ptr<RuntimeContext>   m_runtime_context { nullptr };

    };


}