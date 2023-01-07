#pragma once

#include "runtime/core/logger/logger_system.hpp"
#include "runtime/resource/config/config_system.hpp"
#include "runtime/function/window/window_system.hpp"
#include "runtime/function/render/render_pipeline.hpp"

#include <cassert>
#include <memory>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ARG_TO_XSTR(s) #s
#define ARG_TO_STR(s) ARG_TO_XSTR(s)

namespace Acorn{

    class EngineRuntimeContext{
    public:
        EngineRuntimeContext() = default;
        EngineRuntimeContext(EngineRuntimeContext const&) = delete;
        EngineRuntimeContext const& operator=(EngineRuntimeContext &&) = delete;
        EngineRuntimeContext const& operator=(EngineRuntimeContext const&) = delete;

        virtual void InitSystems() = 0;

        virtual void StartSystems(){
            std::filesystem::current_path(ARG_TO_STR(ENGINE_ROOT_DIR));
            std::cout << "engine root folder : " << std::filesystem::current_path() << "\n";

            assert(m_has_init);

            m_config_system->StartSystem();
            m_logger_system->StartSystem();
            m_window_system->StartSystem();

            m_has_start = true;
        }

        virtual void ShutdownSystems() final{

            assert(m_has_start);
           
            m_window_system->ShutdownSystem();
            m_logger_system->ShutdownSystem();
            m_config_system->ShutdownSystem();

            m_has_start = false;
        }

protected:
        bool m_has_init { false };
        bool m_has_start{ false };
        std::unique_ptr<ConfigSystem>  m_config_system{ nullptr };
        std::unique_ptr<LoggerSystem>  m_logger_system{ nullptr };
        std::unique_ptr<WindowSystem>  m_window_system{ nullptr };
        std::unique_ptr<RenderPipeline> pipeliene;
    };

}
