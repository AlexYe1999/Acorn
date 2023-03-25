#pragma once

#include "window_system.hpp"
#include "3rdparty/glfw/include/GLFW/glfw3.h"

namespace Acorn 
{
    class GlfwWindow : public WindowSystem 
    {
    public:
        GlfwWindow() = default;
        ~GlfwWindow() = default;

        virtual void InitSystem()     override;
        virtual void ShutdownSystem() override;
        virtual void ProcessMessage() override;

        virtual void SetTitle(char const* const text) const override;

        GLFWwindow* GetWindow() { return m_window; }
        
    protected:
        GLFWwindow* m_window { nullptr };
    };


}