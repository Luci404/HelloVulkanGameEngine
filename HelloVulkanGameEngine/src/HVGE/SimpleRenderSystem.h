#pragma once

#include <memory>
#include <vector>

#include "HVGE/Device.h"
#include "HVGE/Pipeline.h"
#include "HVGE/GameObject.h"
#include "HVGE/Camera.h"


namespace HVGE
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem operator=(const SimpleRenderSystem &) = delete;

        void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

    private:
        Device& m_Device;

        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
    };
}