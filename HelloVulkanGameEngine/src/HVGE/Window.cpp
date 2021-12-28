#include "HVGE/Window.h"

#include <assert.h>

namespace HVGE
{
    Window::Window(int width, int height, std::string title)
        : m_Width(width), m_Height(height), m_WindowTitle(title)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);        
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        assert(glfwCreateWindowSurface(instance, m_Window, nullptr, surface) == VK_SUCCESS);
    }
}