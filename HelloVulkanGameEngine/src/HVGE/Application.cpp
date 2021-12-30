#include "HVGE/Application.h"
#include "HVGE/SimpleRenderSystem.h"

#include <assert.h>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace HVGE
{
    Application::Application()
    {
        LoadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        SimpleRenderSystem simpleRenderSystem{m_Device, m_Renderer.GetSwapChainRenderPass()};

        while (!m_Window.ShouldClose())
        {
            glfwPollEvents();

            auto commandBuffer = m_Renderer.BeginFrame();
            m_Renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects);
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
}