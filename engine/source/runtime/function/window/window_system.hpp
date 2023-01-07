#pragma once
#include "runtime/resource/config/config_system.hpp"

#include <cstdint>
#include <vector>
#include <array>
#include <functional>

namespace Acorn{
    class WindowSystem{
    public:
        WindowSystem()  = default;
        ~WindowSystem() = default;
        WindowSystem const& operator=(WindowSystem &&) = delete;
        WindowSystem const& operator=(WindowSystem const&) = delete;

        virtual void InitSystem() = 0;
        virtual void StartSystem() = 0;
        virtual void ShutdownSystem() = 0;

        virtual void ShutdownSystem() = 0; 
        virtual void ProcessMessage() = 0;
        virtual void SetTitle(char const* const) const = 0;

        virtual bool ShouldClose() const { return m_should_close; };
        virtual std::array<uint16_t, 2> GetWindowSize() const { return { m_width, m_height }; }
 
    protected:
        uint16_t m_width   { 1 };
        uint16_t m_height  { 1 };

        bool m_is_fullscreen { false };
        bool m_should_close  { false };

        ConfigSystem* m_config_system { nullptr };

    };
}
