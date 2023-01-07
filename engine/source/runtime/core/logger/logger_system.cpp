#include "logger_system.hpp"
#include "runtime/engine.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Acorn{

    void LoggerSystem::InitSystem(){
        m_config_system = Engine::GetInstance().GetRuntimeContext()->GetConfigSystem();

        m_logger = spdlog::basic_logger_mt("logger", (m_config_system->GetWorkingDirectory() / "log.txt").string());

    }

    void LoggerSystem::StartSystem(){
        
    }

    void LoggerSystem::ShutdownSystem(){
        m_logger->flush();    
    }

}

