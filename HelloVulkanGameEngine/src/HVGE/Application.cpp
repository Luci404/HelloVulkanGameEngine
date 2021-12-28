#include "HVGE/Application.h"

namespace HVGE
{    
    Application::Application()
        : m_Window(1280, 720, "Hello Vulkan Game Engine")
    {}

    Application::~Application() 
    {}

    void Application::Run()
    {
        while (!m_Window.ShouldClose())
        {
            glfwPollEvents();
        }
    }
}