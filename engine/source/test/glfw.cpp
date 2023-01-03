#include "runtime/engine.hpp"
#include "runtime/function/window/glfw_window.hpp"
#include "runtime/function/context/engine_context.hpp"

class TestContext : public Acorn::EngineRuntimeContext
{
public:
    virtual void StartSystems() override
    {
        EngineRuntimeContext::StartSystems();

        window_system = std::make_shared<Acorn::GlfwWindow>();

        Acorn::WindowCreateInfo info;
        window_system->Initialize(info);
    }

    virtual void ShutdownSystems() override
    {
        window_system->ShutdownSystem();
    }

};

int main(int argc, char* argv[])
{
    Acorn::Engine& engine = Acorn::Engine::GetInstance();

    engine.Initialize();
    engine.StartEngine<TestContext>();

    engine.Run();

    engine.ShutdownEngine();
    engine.Clear();
}