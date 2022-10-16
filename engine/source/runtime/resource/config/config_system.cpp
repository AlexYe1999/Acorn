#include "config_system.hpp"
#include "3rdparty/json/include/nlohmann/json.hpp"
#include <fstream>

namespace Acorn{

    void ConfigSystem::InitSystem(){

        if(std::filesystem::is_directory(ARG_TO_XSTR(ENGINE_ROOT_DIR))){
            m_engine_root_path = std::filesystem::path(ARG_TO_XSTR(ENGINE_ROOT_DIR));
            m_working_directory = std::filesystem::current_path();
        }
    }

    void ConfigSystem::StartSystem(){

        std::filesystem::path config_path { m_engine_root_path / "engine_config.ini" };

        if(std::filesystem::exists(config_path)){

            std::ifstream f { config_path };
            nlohmann::json json = nlohmann::json::parse(f);

            m_engine_config.fps    = json["Frame Rate"];
            m_engine_config.width  = json["Width"];
            m_engine_config.height = json["Height"];
            m_engine_config.is_fullscreen = json["Fullscreen"];

        }
    }

    void ConfigSystem::ShutdownSystem(){
    
    }


}
