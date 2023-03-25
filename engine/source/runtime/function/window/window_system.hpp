#pragma once
#include "runtime/resource/config/config_system.hpp"

#include <cstdint>
#include <vector>
#include <tuple>
#include <functional>

namespace Acorn{
    class WindowSystem{
    public:
        WindowSystem()  = default;
        ~WindowSystem() = default;
        WindowSystem const& operator=(WindowSystem &&) = delete;
        WindowSystem const& operator=(WindowSystem const&) = delete;

        virtual void InitSystem()     = 0;
        virtual void ShutdownSystem() = 0;

        virtual void ProcessMessage() = 0;
        virtual void SetTitle(char const* const) const = 0;

        void CloseWindow() { m_should_close = true; } 
        bool ShouldClose() const { return m_should_close; };
        std::tuple<uint16_t, uint16_t> GetWindowSize() const { return { m_width, m_height }; }
 
    protected:
        uint16_t m_width   { 1 };
        uint16_t m_height  { 1 };

        bool m_is_fullscreen { false };
        bool m_should_close  { false };
    };
}
