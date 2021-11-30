#include"Log.hpp"

namespace Acorn{

    void Log::Init(){

        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_spCoreLogger = spdlog::stdout_color_mt("Acorn");
        m_spCoreLogger->set_level(spdlog::level::trace);

        m_spClientLogger = spdlog::stdout_color_mt("App");
        m_spClientLogger->set_level(spdlog::level::trace);
        
    }

}