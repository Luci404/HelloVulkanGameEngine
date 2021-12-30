#pragma once

#include "HVGE/Buffer.h"
#include "HVGE/Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace HVGE
{
	class Model
	{
	public:
		struct Vertex
		{
			glm::vec3 Position{};
			glm::vec3 Color{};
			glm::vec3 Normal{};
			glm::vec2 UV{};

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

			bool operator==(const Vertex& other) const {
				return Position == other.Position && Color == other.Color && Normal == other.Normal && UV == other.UV;
			}
		};

	public:
		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void LoadModel(const std::string& filepath);
		};

		static std::unique_ptr<Model> CreateModelFromFile(Device& device, const std::string& filepath);

		Model(Device& device, const Model::Builder& builder);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32_t>& indices);

	private:
		Device& m_Device;
		std::unique_ptr<Buffer> m_VertexBuffer;
		uint32_t m_VertexCount;

		bool m_HasIndexBuffer = false;
		std::unique_ptr<Buffer> m_IndexBuffer;
		uint32_t m_IndexCount;
	};
}