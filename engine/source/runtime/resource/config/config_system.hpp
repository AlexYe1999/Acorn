#pragma once
#include <filesystem>

#define ARG_TO_XSTR(s) ARG_TO_STR(s)
#define ARG_TO_STR(s) #s

namespace Acorn{

    struct EngineConfig{
        float fps { 1.0f };

        uint16_t width   { 1 };
        uint16_t height  { 1 };

        bool is_fullscreen { false };
    };

    class ConfigSystem{
    public:
        ConfigSystem() = default;
        ConfigSystem(ConfigSystem&&) = default;
        ConfigSystem(ConfigSystem const&) = default;

        virtual void InitSystem();
        virtual void StartSystem();
        virtual void ShutdownSystem();

        std::filesystem::path const& GetEngineRootPath() const {
            return m_engine_root_path;
        }

        std::filesystem::path const& GetWorkingDirectory() const {
            return m_working_directory;
        }

        EngineConfig& GetEngineConfig() { return m_engine_config; }

    protected:
        std::filesystem::path m_engine_root_path;
        std::filesystem::path m_working_directory;

        EngineConfig m_engine_config;

    };

}
