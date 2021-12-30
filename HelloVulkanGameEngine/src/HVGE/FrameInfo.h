#pragma once

#include "HVGE/Camera.h"

#include <vulkan/vulkan.h>

namespace HVGE {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
    VkDescriptorSet globalDescriptorSet;
  };
}