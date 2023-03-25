#pragma once

#include "core/timer/Timer.hpp"
#include "core/logger/logger_system.hpp"
#include "resource/config/config_system.hpp"
#include "function/utils/profiling.hpp"
#include "function/window/window_system.hpp"
#include "function/render/render_system.hpp"

#include <string>
#include <ratio>
#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <algorithm>

namespace Acorn{

    class EngineRuntimeContext
    {
    public:
        EngineRuntimeContext() = default;
        EngineRuntimeContext(EngineRuntimeContext const &) = delete;
        EngineRuntimeContext const &operator=(EngineRuntimeContext &&) = delete;
        EngineRuntimeContext const& operator=(EngineRuntimeContext const&) = delete;

        template <typename T>
        static void SpecifySystems()
        {
            T::SpecifySystems();
        }

        static void InitSystems()
        {
            s_config_system->InitSystem();
            s_logger_system->InitSystem();
            s_window_system->InitSystem();
            s_render_system->InitSystem();
        }

        static void ShutdownSystems()
        {
            s_render_system->ShutdownSystem();
            s_window_system->ShutdownSystem();
            s_logger_system->ShutdownSystem();
            s_config_system->ShutdownSystem();
        }

        static ConfigSystem *ConfigSystem() { return s_config_system.get(); }
        static LoggerSystem *LoggerSystem() { return s_logger_system.get(); }
        static WindowSystem *WindowSystem() { return s_window_system.get(); }
        static RenderSystem *RenderSystem() { return s_render_system.get(); }

    protected:
        inline static std::unique_ptr<class ConfigSystem> s_config_system { nullptr };
        inline static std::unique_ptr<class LoggerSystem> s_logger_system { nullptr };
        inline static std::unique_ptr<class WindowSystem> s_window_system { nullptr };
        inline static std::unique_ptr<class RenderSystem> s_render_system { nullptr };
    };

    class Engine{
        using TickCallback = std::function<void(float)>;

    public:
        Engine(Engine const&) = delete;
        const Engine& operator=(Engine&&) = delete;
        const Engine& operator=(const Engine&) = delete;

        static Engine& Instance(){
            static Engine s_instance;
            return s_instance;
        }

        template<typename RuntimeContext>
        void InitEngine()
        {
            m_engine_timer.Initialize();

            EngineRuntimeContext::SpecifySystems<RuntimeContext>();
            EngineRuntimeContext::InitSystems();
        
            float frame_rate = EngineRuntimeContext::ConfigSystem()->GetEngineConfig().fps;
            m_time_per_frame = 1000.0f / frame_rate;

            m_engine_timer.Tick();
        }

        void ShutdownEngine()
        {
            m_engine_timer.Tick();
            
            EngineRuntimeContext::ShutdownSystems();

            m_engine_timer.Tick();
        }

        void Loop(){
            PROF_THREAD_NAME("Main Thread");
            while(!EngineRuntimeContext::WindowSystem()->ShouldClose()){
                PROF_FUNCTION();
                TickOneFrame();
                PROF_FRAME();
            }
        }
        
        void TickOneFrame()
        {
            float delta_time = m_engine_timer.DeltaTime().count() * 1e-3f;

            static float average_delta_time { 0.0f };
            average_delta_time = average_delta_time * 0.8f + delta_time * 0.2f;

            m_engine_timer.Tick();

            LogicalTick(delta_time);

            RenderTick(delta_time);

            while (m_engine_timer.ElapsedTime().count() < m_time_per_frame) {
                EngineRuntimeContext::WindowSystem()->ProcessMessage();
            }

            std::string runtime_info = "fps : " + std::to_string(1.0f / average_delta_time);

            EngineRuntimeContext::WindowSystem()->SetTitle(runtime_info.c_str());
        }

        void RegisterTickCallback(std::function<void(float)> func)
        {
            m_tick_callbacks.emplace_back(std::move(func));
        }

    protected:

        void LogicalTick(float delta_time)
        {
            PROF_FUNCTION();

            for (auto& tick : m_tick_callbacks)
            {
                tick(delta_time);
            }
        }

        void RenderTick(float delta_time)
        {
            PROF_FUNCTION();
            EngineRuntimeContext::RenderSystem()->Tick(delta_time);
        }

    private:
        Engine() = default;
        virtual ~Engine() = default;

        float                     m_time_per_frame { 1.0f };
        Timer<float, std::milli>  m_engine_timer   {};
        std::vector<TickCallback> m_pre_tick_callbacks {};
        std::vector<TickCallback> m_tick_callbacks {};
        std::vector<TickCallback> m_post_tick_callbacks{};
    };


}