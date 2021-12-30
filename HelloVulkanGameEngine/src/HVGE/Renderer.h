#pragma once

#include "HVGE/Window.h"
#include "HVGE/Device.h"
#include "HVGE/SwapChain.h"

#include <memory>
#include <vector>
#include <assert.h>

namespace HVGE
{
    class Renderer
    {
    public:
        Renderer(Window& window, Device& device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer operator=(const Renderer&) = delete;

        VkRenderPass GetSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
        bool IsFrameInProgress() const { return m_IsFrameStarted; }
        VkCommandBuffer GetCurrentCommandBuffer() const { assert(m_IsFrameStarted); return m_CommandBuffers[m_CurrentFrameIndex]; }
        int GetFrameIndex() const { assert(m_IsFrameStarted); return m_CurrentFrameIndex; }

        VkCommandBuffer BeginFrame();
        void EndFrame();
        void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void FreeCommandBuffers();
        void CreateCommandBuffers();
        void RecreateSwapChain();

    private:
        Window& m_Window;
        Device& m_Device;
        std::unique_ptr<SwapChain> m_SwapChain;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        uint32_t m_CurrentImageIndex;
        int m_CurrentFrameIndex = 0;
        bool m_IsFrameStarted = false;
    };
}