#include "Objects2D.h"

#include <Core/Engine.h>

Mesh* Objects2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Objects2D::CreateBall(std::string name, glm::vec3 centerCircle, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = centerCircle;

	std::vector<VertexFormat> vertices{};
	float i;
	float pi = 3.14f;
	float angle = 2 * pi / 256;

	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));

	for (i = 0; i <= 2 * 3.14; i += angle) {

		vertices.push_back(VertexFormat(corner + glm::vec3(length * cos(i), length * sin(i), 0), color));
	}

	std::vector<unsigned short> indices = {	};

	int j;
	for (j = 1; j <= 255; j++) {
		indices.push_back(0);
		indices.push_back(j);
		indices.push_back(j + 1);
	}

	indices.push_back(0);
	indices.push_back(256);
	indices.push_back(1);

	Mesh* circle = new Mesh(name);
	circle->InitFromData(vertices, indices);
	return circle;
}