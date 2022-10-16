#include "runtime/engine.hpp"
#include "runtime/function/context/engine_context.hpp"
#include "runtime/core/logger/logger_system.hpp"
#include "runtime/resource/config/config_system.hpp"
#include "runtime/function/window/win32_window.hpp"

using namespace Acorn;

class Win32TestContext : public RuntimeContext{
public: 

    virtual void InitSystems() override{

        m_config_system = std::make_unique<ConfigSystem>();
        m_config_system->InitSystem();

        m_logger_system = std::make_unique<LoggerSystem>();
        m_logger_system->InitSystem();

        m_window_system = std::make_unique<Win32Window>();
        m_window_system->InitSystem();

        m_has_init = true;
    }

};

int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, int nCmdShow
){

    Acorn::Engine& engine = Acorn::Engine::GetInstance();

    engine.InitEngine<Win32TestContext>();
    engine.StartEngine();

    engine.Run();

    engine.ShutdownEngine();
}