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
	struct GlobalUbo {
		glm::mat4 projectionView{ 1.f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.f, -3.f, -1.f });
	};

	Application::Application()
	{
		LoadGameObjects();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Buffer globalUboBuffer{  m_Device, sizeof(GlobalUbo), SwapChain::MAX_FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_Device.properties.limits.minUniformBufferOffsetAlignment };
		globalUboBuffer.map();

		SimpleRenderSystem simpleRenderSystem{ m_Device, m_Renderer.GetSwapChainRenderPass() };
        Camera camera{};

		auto viewerObject = GameObject::CreateGameObject();
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
            camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			auto commandBuffer = m_Renderer.BeginFrame();

			int frameIndex = m_Renderer.GetFrameIndex();
			FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera };
			
			// Update
			GlobalUbo ubo{};
			ubo.projectionView = camera.GetProjection() * camera.GetView();
			globalUboBuffer.writeToIndex(&ubo, frameIndex);
			globalUboBuffer.flushIndex(frameIndex);

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
		flatVaseGameObject.transform.translation = { -.5f, .5f, 2.5f };
		flatVaseGameObject.transform.scale = { 3.f, 1.5f, 3.f };
		m_GameObjects.push_back(std::move(flatVaseGameObject));

		std::shared_ptr<Model> smoothVaseModel = Model::CreateModelFromFile(m_Device, "C:/Dev/HelloVulkanGameEngine/HelloVulkanGameEngine/assets/models/smooth_vase.obj");
		auto smoothVaseGameObject = GameObject::CreateGameObject();
		smoothVaseGameObject.model = smoothVaseModel;
		smoothVaseGameObject.transform.translation = { .5f, .5f, 2.5f };
		smoothVaseGameObject.transform.scale = { 3.f, 1.5f, 3.f };
		m_GameObjects.push_back(std::move(smoothVaseGameObject));
	}
}