#pragma once

#include "HVGE/Window.h"
#include "HVGE/Device.h"
#include "HVGE/SwapChain.h"
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
        void CreatePipelineLayout();
        void CreatePipeline();
        void FreeCommandBuffers();
        void CreateCommandBuffers();
        void DrawFrame();
        void RecreateSwapChain();
        void RecordCommandBuffer(int imageIndex);
        void RenderGameObjects(VkCommandBuffer m_CommandBuffer);

    private:
        Window m_Window{ 1280, 720, "Hello Vulkan Game Engine" };
        Device m_Device{ m_Window };
        std::unique_ptr<SwapChain> m_SwapChain;
        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        std::vector<GameObject> m_GameObjects;
    };
}