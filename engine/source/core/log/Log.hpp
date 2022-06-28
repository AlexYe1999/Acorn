#pragma once
#include"spdlog/spdlog.h"
#include"spdlog/common.h"
#include"spdlog/sinks/stdout_color_sinks.h"
#include<memory>

namespace Acorn{

    class __declspec(dllexport) Log{
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_spCoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_spClientLogger; }

        Log() = delete;
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

    private:
        inline static std::shared_ptr<spdlog::logger> m_spCoreLogger;
        inline static std::shared_ptr<spdlog::logger> m_spClientLogger;

    };

}