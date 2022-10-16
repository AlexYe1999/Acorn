#pragma once

#include "runtime/core/logger/logger_system.hpp"
#include "runtime/resource/config/config_system.hpp"
#include "runtime/function/window/window_system.hpp"

#include <cassert>
#include <memory>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ARG_TO_STR(s) #s

namespace Acorn{

    class RuntimeContext{
    public:

        RuntimeContext() = default;
        RuntimeContext(RuntimeContext const&) = delete;
        RuntimeContext const& operator=(RuntimeContext &&) = delete;
        RuntimeContext const& operator=(RuntimeContext const&) = delete;

        virtual void InitSystems() = 0;

        virtual void StartSystems() final{

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

        bool HasSystemInit() const{
            return m_has_init;
        }

        bool HasSystemStart() const{
            return m_has_start;
        }

        ConfigSystem* GetConfigSystem() {
            return m_config_system.get();
        }

        LoggerSystem* GetLoggerSystem() {
            return m_logger_system.get();
        }

        WindowSystem* GetWindowSystem() {
            return m_window_system.get();
        }

    protected:
        bool m_has_init  { false };
        bool m_has_start { false };
        std::unique_ptr<ConfigSystem>  m_config_system { nullptr };
        std::unique_ptr<LoggerSystem>  m_logger_system { nullptr };
        std::unique_ptr<WindowSystem>  m_window_system { nullptr };
    };

}
