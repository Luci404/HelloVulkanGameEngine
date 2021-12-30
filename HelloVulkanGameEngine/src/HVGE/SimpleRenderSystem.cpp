#include "HVGE/SimpleRenderSystem.h"

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
    SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass)
        : m_Device(device)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void SimpleRenderSystem::CreatePipelineLayout()
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

    void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        assert(m_PipelineLayout != nullptr);

        PipelineConfigInfo pipelineConfig{};
        Pipeline::DefaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_PipelineLayout;

        m_Pipeline = std::make_unique<Pipeline>(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.vert.spv", "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/shaders/shader.frag.spv", pipelineConfig);
    }

    void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects)
    {
        m_Pipeline->Bind(commandBuffer);

        for (auto &obj : gameObjects)
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