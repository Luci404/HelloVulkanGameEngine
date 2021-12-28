#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace HVGE
{
    class Window
    {
    public:
        Window(int width, int height, std::string title);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool ShouldClose() { return glfwWindowShouldClose(m_Window); }

        VkExtent2D GetExtent() { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        int m_Width;
        int m_Height;
        std::string m_WindowTitle;

        GLFWwindow* m_Window;
    };
}