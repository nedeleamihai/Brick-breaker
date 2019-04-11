#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBall(std::string name, glm::vec3 centerCircle, float length, glm::vec3 color, bool fill = false);
}

