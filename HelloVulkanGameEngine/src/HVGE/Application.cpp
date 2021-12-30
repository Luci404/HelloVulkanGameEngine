#include "HVGE/Application.h"

#include <assert.h>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "HVGE/KeyboardMovementController.h"
#include "HVGE/SimpleRenderSystem.h"
#include "HVGE/Camera.h"
#include "HVGE/Buffer.h"

namespace HVGE
{
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.f};
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
        glm::vec3 lightPosition{ -1.f };
        alignas(16) glm::vec4 lightColor{ 1.f };  // w is light intensity
    };

    Application::Application()
    {
        globalPool = DescriptorPool::Builder(m_Device)
                         .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                         .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                         .build();
        LoadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<Buffer>(m_Device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(m_Device)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                                   .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{m_Device, m_Renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        Camera camera{};

        auto viewerObject = GameObject::CreateGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!m_Window.ShouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.MoveInPlaneXZ(m_Window.GetGLFWwindow(), frameTime, viewerObject);
            camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = m_Renderer.GetAspectRatio();
            camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            auto commandBuffer = m_Renderer.BeginFrame();

            int frameIndex = m_Renderer.GetFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

            // Update
            GlobalUbo ubo{};
            ubo.projectionView = camera.GetProjection() * camera.GetView();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // Render
            m_Renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.RenderGameObjects(frameInfo, m_GameObjects);
            m_Renderer.EndSwapChainRenderPass(commandBuffer);
            m_Renderer.EndFrame();
        }

        vkDeviceWaitIdle(m_Device.device());
    }

    void Application::LoadGameObjects()
    {
        std::shared_ptr<Model> flatVaseModel = Model::CreateModelFromFile(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/models/flat_vase.obj");
        auto flatVaseGameObject = GameObject::CreateGameObject();
        flatVaseGameObject.model = flatVaseModel;
        flatVaseGameObject.transform.translation = {-.5f, .5f, 0.0f};
        flatVaseGameObject.transform.scale = {3.f, 1.5f, 3.f};
        m_GameObjects.push_back(std::move(flatVaseGameObject));

        std::shared_ptr<Model> smoothVaseModel = Model::CreateModelFromFile(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/models/smooth_vase.obj");
        auto smoothVaseGameObject = GameObject::CreateGameObject();
        smoothVaseGameObject.model = smoothVaseModel;
        smoothVaseGameObject.transform.translation = {.5f, .5f, 0.0f};
        smoothVaseGameObject.transform.scale = {3.f, 1.5f, 3.f};
        m_GameObjects.push_back(std::move(smoothVaseGameObject));

        std::shared_ptr<Model> cubeModel = Model::CreateModelFromFile(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/models/quad.obj");
        auto cubeGameObject = GameObject::CreateGameObject();
        cubeGameObject.model = cubeModel;
        cubeGameObject.transform.translation = { 0.0f, .5f, 0.0f };
        cubeGameObject.transform.scale = { 3.f, 1.0f, 3.f };
        m_GameObjects.push_back(std::move(cubeGameObject));
    }
}