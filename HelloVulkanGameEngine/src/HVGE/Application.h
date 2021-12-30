#pragma once

#include "HVGE/Window.h"
#include "HVGE/Device.h"
#include "HVGE/Renderer.h"
#include "HVGE/Pipeline.h"
#include "HVGE/GameObject.h"

#include <memory>
#include <vector>

namespace HVGE
{
    class Application
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application operator=(const Application&) = delete;

        void Run();

    private:
        void LoadGameObjects();

    private:
        Window m_Window{ 1280, 720, "Hello Vulkan Game Engine" };
        Device m_Device{ m_Window };
        Renderer m_Renderer {m_Window, m_Device };

        std::vector<GameObject> m_GameObjects;
    };
}