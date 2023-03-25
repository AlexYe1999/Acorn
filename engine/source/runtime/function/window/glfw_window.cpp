#include "glfw_window.hpp"
#include "runtime/engine.hpp"

namespace Acorn
{
	void GlfwWindow::InitSystem()
	{
        const auto& config = EngineRuntimeContext::ConfigSystem()->GetEngineConfig();
        m_width  = config.width;
        m_height = config.height;
        m_is_fullscreen = config.is_fullscreen;

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_width, m_height, "", nullptr, nullptr);
	}

    void GlfwWindow::ShutdownSystem()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void GlfwWindow::ProcessMessage()
	{
		glfwPollEvents();
		m_should_close = glfwWindowShouldClose(m_window);
	}

	void GlfwWindow::SetTitle(char const* const text) const
	{
		glfwSetWindowTitle(m_window, text);
	}


}


