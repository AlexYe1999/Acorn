#pragma once

#include "runtime/engine.hpp"
#include "runtime/function/window/win32_window.hpp"
#include "runtime/resource/config/config_system.hpp"
#include "runtime/core/logger/logger_system.hpp"

namespace Acorn
{
    class EditorContext : public EngineRuntimeContext
    {
    public:
        static void SpecifySystems()
        {
            s_config_system = std::make_unique<class ConfigSystem>();
            s_logger_system = std::make_unique<class LoggerSystem>();
            s_window_system = std::make_unique<class Win32Window>();
            s_render_system = std::make_unique<class RenderSystem>();
        }

    };

    class Editor
    {
    public:
        Editor(Editor const&) = delete;
        const Editor& operator=(Editor&&) = delete;
        const Editor& operator=(const Editor&) = delete;

        static Editor& Instance()
        {
            static Editor editor;
            return editor;
        }

        void Run();

    private:
        Editor() = default;

        void Tick(float delta_time);

        void Init();
        void Shutdown();

        void InitImGui();
        void UpdateImGui(float delta_time);

    };
}