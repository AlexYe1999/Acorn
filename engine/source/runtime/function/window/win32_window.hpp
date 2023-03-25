#pragma once

#include"runtime/function/window/window_system.hpp"

#include <windows.h>

namespace Acorn{

    class Win32Window final : public WindowSystem{
    public:
        Win32Window()  = default;
        ~Win32Window() = default;

        virtual void InitSystem() override;
        virtual void ShutdownSystem() override;

        virtual void ProcessMessage() override;

        virtual void SetTitle(char const* const text) const override;
        
        HWND Hwnd() { return m_hwnd; }

    protected:
        HWND m_hwnd{};
    };


}
