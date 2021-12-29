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
        bool WasWindowResized() { return m_FramebufferResized;  }
        void ResetWindowResized() { m_FramebufferResized = false; }

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    
    private:
        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

        int m_Width;
        int m_Height;
        bool m_FramebufferResized;
        std::string m_WindowTitle;

        GLFWwindow* m_Window;
    };
}