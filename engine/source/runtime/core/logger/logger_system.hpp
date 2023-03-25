#pragma once

#include "runtime/resource/config/config_system.hpp"
#include "spdlog/logger.h"

#include <memory>

namespace Acorn{

    class LoggerSystem{
    public:
        LoggerSystem() = default;
        LoggerSystem(LoggerSystem &&) = delete;
        LoggerSystem(LoggerSystem const&) = delete;
        LoggerSystem const& operator=(LoggerSystem&&) = delete;
        LoggerSystem const& operator=(LoggerSystem const&) = delete;

        virtual void InitSystem();
        virtual void ShutdownSystem();

    protected:
        std::shared_ptr<spdlog::logger> m_logger { nullptr };
    };

}
