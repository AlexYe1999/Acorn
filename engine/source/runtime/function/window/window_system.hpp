#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <functional>

namespace Acorn{

    struct WindowCreateInfo{
        uint16_t    width  { 1280 };
        uint16_t    height { 720 };
        const char* title  { "Acorn" };
        bool        is_fullscreen { false };
    };
    
    class WindowSystem{
    public:
        WindowSystem()  = default;
        ~WindowSystem() = default;

        virtual void Initialize(WindowCreateInfo const& createInfo){
            m_width  = createInfo.width;
            m_height = createInfo.height;
            m_is_fullscreen = createInfo.is_fullscreen;
        }

        virtual void ShutdownSystem() = 0; 
        virtual void ProcessMessage() = 0;

        virtual bool ShouldClose() const{ return m_should_close; };

        virtual std::array<uint16_t, 2> GetWindowSize() const{ return { m_width, m_height }; }
 
        virtual void SetTitle(char const* const) const = 0;

    protected:
        uint16_t m_width   { 0 };
        uint16_t m_height  { 0 };

        bool m_should_close  { false };
        bool m_is_fullscreen { false };

    };
}
