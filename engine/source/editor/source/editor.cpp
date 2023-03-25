#include "editor.hpp"
#include "runtime/engine.hpp"

#include <iostream>
#include <functional>

namespace Acorn
{
    void Editor::Run()
    {
        Init();

        Engine::Instance().Loop();

        Shutdown();
    }

    void Editor::Tick(float delta_time)
    {
        UpdateImGui(delta_time);
    }

    void Editor::Init()
    {
        Engine::Instance().InitEngine<EditorContext>();
        Engine::Instance().RegisterTickCallback(std::bind(&Editor::Tick, this, std::placeholders::_1));

        InitImGui();
    }

    void Editor::Shutdown()
    {
        Engine::Instance().ShutdownEngine();
    }

    void Editor::InitImGui()
    {

    }

    void Editor::UpdateImGui(float delta_time)
    {
        //std::cout << delta_time << std::endl;
    }
}