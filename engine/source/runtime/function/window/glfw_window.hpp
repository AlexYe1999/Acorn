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

        virtual void Initialize(WindowCreateInfo const& createInfo) override;
        virtual void ShutdownSystem() override;
        virtual void ProcessMessage() override;

        virtual void SetTitle(char const* const text) const override;

    protected:
        GLFWwindow* m_window { nullptr };
    };


}