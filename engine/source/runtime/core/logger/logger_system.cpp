#include "logger_system.hpp"
#include "runtime/engine.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Acorn
{
    void LoggerSystem::InitSystem()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_logger = spdlog::basic_logger_mt("logger", (EngineRuntimeContext::ConfigSystem()->GetWorkingDirectory() / "log.txt").string());
        m_logger->set_level(spdlog::level::trace);
        m_logger->info("start logger");
    }

    void LoggerSystem::ShutdownSystem()
    {
        m_logger->flush();    
    }
}

