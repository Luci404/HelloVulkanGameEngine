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
			m_Renderer.BeginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects, camera);
			m_Renderer.EndSwapChainRenderPass(commandBuffer);
			m_Renderer.EndFrame();
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	// temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
	std::unique_ptr<Model> CreateCubeModel(Device& device, glm::vec3 offset) {
		Model::Builder modelBuilder{};
		modelBuilder.vertices = {
			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : modelBuilder.vertices) {
			v.Position += offset;
		}

		modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<Model>(device, modelBuilder);
	}

	void Application::LoadGameObjects()
	{
		std::shared_ptr<Model> cubeModel = CreateCubeModel(m_Device, { 0.0f, 0.0f, 0.0f });
		auto cubeGameObject = GameObject::CreateGameObject();
		cubeGameObject.model = cubeModel;
		cubeGameObject.transform.translation = { 0.0f, 0.0f, 2.5f };
		cubeGameObject.transform.scale = { 0.5f, 0.5f, 0.5f };
		m_GameObjects.push_back(std::move(cubeGameObject));
	}
}