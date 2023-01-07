#include "runtime/engine.hpp"
#include "runtime/function/context/engine_context.hpp"
#include "runtime/core/logger/logger_system.hpp"
#include "runtime/resource/config/config_system.hpp"
#include "runtime/function/window/win32_window.hpp"

using namespace Acorn;

class TestContext : public Acorn::EngineRuntimeContext{
public: 

    virtual void InitSystems() override{

        EngineRuntimeContext::StartSystems();

        m_config_system = std::make_unique<ConfigSystem>();
        m_config_system->InitSystem();

        m_logger_system = std::make_unique<LoggerSystem>();
        m_logger_system->InitSystem();

        window_system = std::make_shared<Acorn::Win32Window>();

        Acorn::WindowCreateInfo info;
        window_system->Initialize(info);
    }

    virtual void ShutdownSystems() override{}

};

int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, int nCmdShow
){

    Acorn::Engine& engine = Acorn::Engine::GetInstance();

    engine.Initialize();
    engine.StartEngine<TestContext>();

    engine.Run();

    engine.ShutdownEngine();
}