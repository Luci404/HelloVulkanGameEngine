#include "HVGE/Application.h"

#include <assert.h>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct SimplePushConstantData
{
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

namespace HVGE
{
    Application::Application()
    {
        LoadGameObjects();
        CreatePipelineLayout();
        CreatePipeline();
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

            auto commandBuffer = m_Renderer.BeginFrame();
            m_Renderer.BeginSwapChainRenderPass(commandBuffer);
            RenderGameObjects(commandBuffer);
            m_Renderer.EndSwapChainRenderPass(commandBuffer);
            m_Renderer.EndFrame();
        }

        vkDeviceWaitIdle(m_Device.device());
    }

    void Application::LoadGameObjects()
    {
        std::vector<Model::Vertex> triangleVertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

        auto triangleModel = std::make_shared<Model>(m_Device, triangleVertices);

        auto triangleGameObject = GameObject::CreateGameObject();
        triangleGameObject.model = triangleModel;
        triangleGameObject.color = {.1f, .8f, .1f};
        triangleGameObject.transform2d.translation.x = .2f;
        triangleGameObject.transform2d.scale = {2.f, .5f};
        triangleGameObject.transform2d.rotation = .25f * glm::two_pi<float>();

        m_GameObjects.push_back(std::move(triangleGameObject));
    }

    void Application::CreatePipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        assert(vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS);
    }

    void Application::CreatePipeline()
    {
        assert(m_PipelineLayout != nullptr);

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = m_Renderer.GetSwapChainRenderPass();
        pipelineConfig.pipelineLayout = m_PipelineLayout;

        m_Pipeline = std::make_unique<Pipeline>(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.vert.spv", "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.frag.spv", pipelineConfig);
    }

    void Application::RenderGameObjects(VkCommandBuffer commandBuffer)
    {
        m_Pipeline->Bind(commandBuffer);

        for (auto &obj : m_GameObjects)
        {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            obj.model->Bind(commandBuffer);
            obj.model->Draw(commandBuffer);
        }
    }
}