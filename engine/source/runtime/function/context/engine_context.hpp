#pragma once

#include "runtime/function/window/window_system.hpp"

#include <memory>
#include <filesystem>

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define PILOT_XSTR(s) PILOT_STR(s)
#define PILOT_STR(s) #s

namespace Acorn{

    struct EngineInitParams{
        std::filesystem::path root_folder;
        //std::filesystem::path config_file_path;
    };

    class EngineRuntimeContext{
    public:

        virtual void StartSystems(){
            std::filesystem::path pilot_root_folder = std::filesystem::path(PILOT_XSTR(ACORN_ROOT_DIR));
            m_engine_init_param.root_folder = pilot_root_folder;
        }

        virtual void ShutdownSystems() = 0;

        EngineInitParams m_engine_init_param;
        std::shared_ptr<WindowSystem> m_window_system;
    };

}
