#pragma once

#include "HVGE/Camera.h"
#include "HVGE/GameObject.h"

#include <vulkan/vulkan.h>

namespace HVGE {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map& gameObjects;
	};
}