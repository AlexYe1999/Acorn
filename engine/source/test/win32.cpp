#include "runtime/engine.hpp"
#include "runtime/function/window/win32_window.hpp"
#include "runtime/function/context/engine_context.hpp"

class Win32TestContext : public Acorn::EngineRuntimeContext{
public: 
    virtual void StartSystems() override{
        EngineRuntimeContext::StartSystems();

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
    engine.StartEngine<Win32TestContext>();

    engine.Run();

    engine.ShutdownEngine();
    engine.Clear();
}