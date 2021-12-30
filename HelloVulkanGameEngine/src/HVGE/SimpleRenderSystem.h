#pragma once

#include <memory>
#include <vector>

#include "HVGE/Device.h"
#include "HVGE/Pipeline.h"
#include "HVGE/GameObject.h"
#include "HVGE/Camera.h"
#include "HVGE/FrameInfo.h"


namespace HVGE
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem operator=(const SimpleRenderSystem &) = delete;

        void RenderGameObjects(FrameInfo &frameInfo, std::vector<GameObject>& gameObjects);

    private:
        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

    private:
        Device& m_Device;

        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
    };
}