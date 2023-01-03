#pragma once

#include "window_system.hpp"
#include <windows.h>

namespace Acorn{

    class Win32Window : public WindowSystem{
    public:
        Win32Window()  = default;
        ~Win32Window() = default;

        virtual void Initialize(WindowCreateInfo const& createInfo) override;
        virtual void ShutdownSystem() override; 
        virtual void ProcessMessage() override;

        virtual void SetTitle(char const* const text) const override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:
        HWND m_hwnd{};
    };


}
