#include "HVGE/Renderer.h"

#include <assert.h>
#include <array>

namespace HVGE
{
    Renderer::Renderer(Window &window, Device &device)
        : m_Window(window), m_Device(device)
    {
        RecreateSwapChain();
        CreateCommandBuffers();
    }

    Renderer::~Renderer()
    {
        FreeCommandBuffers();
    }

    VkCommandBuffer Renderer::BeginFrame()
    {
        assert(!m_IsFrameStarted);

        auto result = m_SwapChain->acquireNextImage(&m_CurrentImageIndex);
        assert(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
        }

        m_IsFrameStarted = true;

        auto commandBuffer = GetCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        assert(vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS);

        return commandBuffer;

    }

    void Renderer::EndFrame()
    {
        assert(m_IsFrameStarted);
        auto commandBuffer = GetCurrentCommandBuffer();

        assert(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS);

        auto result = m_SwapChain->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
        assert(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR);

        if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR || m_Window.WasWindowResized())
        {
            m_Window.ResetWindowResized();
            RecreateSwapChain();
        }

        m_IsFrameStarted = false;
    }

    void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(m_IsFrameStarted);
        assert(commandBuffer == GetCurrentCommandBuffer());

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain->getRenderPass();
        renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_CurrentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, m_SwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(m_IsFrameStarted);
        assert(commandBuffer == GetCurrentCommandBuffer());

        vkCmdEndRenderPass(commandBuffer);
    }

    void Renderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(m_Device.device(), m_Device.getCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
        m_CommandBuffers.clear();
    }

    void Renderer::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(m_SwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

        assert(vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) == VK_SUCCESS);
    }

    void Renderer::RecreateSwapChain()
    {
        auto extent = m_Window.GetExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            glfwWaitEvents();
            extent = m_Window.GetExtent();
        }

        vkDeviceWaitIdle(m_Device.device());

        if (m_SwapChain == nullptr)
        {
            m_SwapChain = std::make_unique<SwapChain>(m_Device, extent);
        }
        else
        {
            m_SwapChain = std::make_unique<SwapChain>(m_Device, extent, std::move(m_SwapChain));
            if (m_SwapChain->imageCount() != m_CommandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }
    }
}