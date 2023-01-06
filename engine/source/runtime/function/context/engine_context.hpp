#pragma once

#include "runtime/function/window/window_system.hpp"
#include "runtime/function/render/render_pipeline.hpp"

#include <memory>
#include <iostream>
#include <filesystem>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ARG_TO_XSTR(s) #s
#define ARG_TO_STR(s) ARG_TO_XSTR(s)

namespace Acorn
{
    class EngineRuntimeContext
    {
    public:
        virtual void StartSystems()
        {
            std::filesystem::current_path(ARG_TO_STR(ENGINE_ROOT_DIR));
            std::cout << "engine root folder : " << std::filesystem::current_path() << "\n";
        }

        virtual void ShutdownSystems() = 0;

        std::unique_ptr<WindowSystem>   window_system;
        std::unique_ptr<RenderPipeline> pipeliene;
    };

}
