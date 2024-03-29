#include "runtime/engine.hpp"
#include "runtime/function/window/win32_window.hpp"
#include "runtime/function/context/engine_context.hpp"

class TestContext : public Acorn::EngineRuntimeContext{
public: 
    virtual void StartSystems() override{
        EngineRuntimeContext::StartSystems();

        window_system = std::make_unique<Acorn::Win32Window>();

        Acorn::WindowCreateInfo info;
        window_system->Initialize(info);

    }

    virtual void ShutdownSystems() override
    {
        window_system->ShutdownSystem();
    }

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
    engine.Clear();
}