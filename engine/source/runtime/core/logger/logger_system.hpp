#pragma once

#include "runtime/resource/config/config_system.hpp"
#include "spdlog/logger.h"

namespace Acorn{

    class LoggerSystem{
    public:
        LoggerSystem() = default;
        LoggerSystem(LoggerSystem &&) = delete;
        LoggerSystem(LoggerSystem const&) = delete;
        LoggerSystem const& operator=(LoggerSystem&&) = delete;
        LoggerSystem const& operator=(LoggerSystem const&) = delete;

        virtual void InitSystem();
        virtual void StartSystem();
        virtual void ShutdownSystem();

    protected:
        ConfigSystem* m_config_system { nullptr };
        spdlog::logger m_logger;
    };

}
