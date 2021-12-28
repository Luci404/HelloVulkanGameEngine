#include "HVGE/Application.h"

#include <assert.h>

namespace HVGE
{    
    Application::Application()
        : m_Window(1280, 720, "Hello Vulkan Game Engine"), m_Device(m_Window), m_SwapChain(m_Device, m_Window.GetExtent())
    {
        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();
    }

    Application::~Application() 
    {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void Application::Run()
    {
        while (!m_Window.ShouldClose())
        {
            glfwPollEvents();
        }
    }

    void Application::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        createInfo.setLayoutCount = 0;
        createInfo.pSetLayouts = nullptr;
        createInfo.pushConstantRangeCount = 0;
        createInfo.pPushConstantRanges = nullptr;

        assert(vkCreatePipelineLayout(m_Device.device(), &createInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS);
    }

    void Application::CreatePipeline()
    {
        PipelineConfigInfo pipelineConfig = Pipeline::DefaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height());
        pipelineConfig.renderPass = m_SwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = m_PipelineLayout;

        m_Pipeline = std::make_unique<Pipeline>(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.vert.spv", "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.frag.spv", pipelineConfig);
    }

    void Application::CreateCommandBuffers()
    {

    }

    void Application::DrawFrame()
    {

    }
}