#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>

#include "HVGE/Application.h"

int main(int argc, char* argv[])
{
	HVGE::Application application;
	
	application.Run();

	return 0;
}