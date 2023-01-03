#include "glfw_window.hpp"

namespace Acorn
{
	void GlfwWindow::Initialize(WindowCreateInfo const& createInfo)
	{
		WindowSystem::Initialize(createInfo);

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_width, m_height, createInfo.title, nullptr, nullptr);
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


