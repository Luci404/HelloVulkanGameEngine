#pragma once

#include "HVGE/Device.h"
#include "HVGE/Pipeline.h"
#include "HVGE/GameObject.h"

#include <memory>
#include <vector>

namespace HVGE
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem operator=(const SimpleRenderSystem &) = delete;

        void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);

    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

    private:
        Device& m_Device;

        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
    };
}