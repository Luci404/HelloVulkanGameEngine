#pragma once

#include "HVGE/Window.h"
#include "HVGE/Device.h"
#include "HVGE/SwapChain.h"
#include "HVGE/Pipeline.h"

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
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();

        void DrawFrame();

    private:
        Window m_Window;
        Device m_Device;
        SwapChain m_SwapChain;
        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<VkCommandBuffer> m_CommandBuffers;
    };
}