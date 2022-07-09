#pragma once

#include "runtime/function/context/engine_context.hpp"

#include <chrono>
#include <string>
#include <unordered_set>


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

            m_runtime_context = std::make_unique<RuntimeContext>();
            m_runtime_context->StartSystems();

        }

        void ShutdownEngine(){
            m_runtime_context->ShutdownSystems();
        }

        void Run(){

            while(!m_runtime_context->m_window_system->ShouldClose()){
                float tick_time = TimerTick();
                TickOneFrame(tick_time);
            }

        }

        bool TickOneFrame(float delta_time){

            LogicalTick(delta_time);

            RendererTick();

            auto window = m_runtime_context->m_window_system;
            window->ProcessMessage();
            window->SetTitle(std::string(std::to_string(GetFPS()) + " FPS").c_str());

            return !window->ShouldClose();
        }

        float GetFPS() const {
            return m_fps; 
        }

        EngineRuntimeContext* GetEngineRuntimeContext() const{
            return m_runtime_context.get();
        }

    protected:
        void LogicalTick(float delta_time){

        }

        void RendererTick(){

        }

        float TimerTick(){
            using namespace std::chrono;

            static uint16_t frame_count   { 0 };
            static float average_duration { 0.0f };
            static steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };

            frame_count++;
            steady_clock::time_point tick_time_point = steady_clock::now();
            float delta_time = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point).count();

            m_last_tick_time_point = tick_time_point;

            average_duration = average_duration * 99e-2f + delta_time * 1e-2f;
            m_fps = static_cast<int>(1.f / average_duration);

            return delta_time;
        }

    protected:
        Engine() = default;
        virtual ~Engine() = default;

        uint16_t m_fps = 0;
        std::unique_ptr<EngineRuntimeContext> m_runtime_context;

    };


}