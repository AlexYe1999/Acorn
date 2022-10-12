#pragma once

#include "runtime/function/window/window_system.hpp"

#include <memory>
#include <iostream>
#include <filesystem>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define ARG_TO_STR(s) #s

namespace Acorn{

    class EngineRuntimeContext{
    public:

        virtual void StartSystems(){
            std::filesystem::path engine_root_folder = std::filesystem::path(ARG_TO_STR(ACORN_ROOT_DIR));

            std::cout << "engine root folder : " << engine_root_folder << "\n";
        }

        virtual void ShutdownSystems() = 0;

        std::shared_ptr<WindowSystem> window_system;
    };

}
