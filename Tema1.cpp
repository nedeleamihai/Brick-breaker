#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Objects2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	//SteaNinja
	const string textureLoc = "Source/Laboratoare/Tema1/Textures/";
	Texture2D* texture = new Texture2D();
	texture->Load2D((textureLoc + "NinjaStar.png").c_str(), GL_REPEAT);
	vector<VertexFormat> vertices2
	{
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.2, 0.7, 0.9)),
		VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.1, 1, 1)),
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.1, 0.6 , 1)),
		VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0.1, 0.5, 1))
	};

	vector<unsigned short> indices2 =
	{
		0, 1, 2,
		3, 2, 0
	};
	//Mesh *patrat = CreateMesh("patrat", vertices2, indices2);
	//meshes[patrat->GetMeshID()] = patrat;

	glm::vec3 corner1 = glm::vec3(0, 80, 0);
	glm::vec3 corner2 = glm::vec3(0, resolution.y - 25, 0);
	glm::vec3 corner3 = glm::vec3(resolution.x - 25, 80, 0);
	glm::vec3 element = glm::vec3(0, 0, 0);

	Mouse_pressed = false;
	NrVieti = 3;
	PowerUpTime = 0;
	powersUp.clear();
	bricks.clear();

	// Initializeaza latimea si lungimea caramizilor
	width = 10;
	length = 200;
	widthBrick = 40;
	lengthBrick = 60;

	// Initializeaza raza bilei si latura patratului
	radius = 10;
	squareSide = 20;

	// Initializeaza pasii de translatie
	translateX = resolution.x/2.0f;
	translateY = 60;
	translateSignX = 1;
	translateSignY = 1;
	platformaX = resolution.x / 2.0f - 100.0f;
	platformaY = 20;

	// Initializeaza factorii de scalare
	scaleX = 1;
	scaleY = 1;
	
	// Initializeaza unghiul de rotatie
	angularStep = 0;	

	Mesh* p0 = Objects2D::CreateRectangle("down", element, resolution.x, 10.0f, glm::vec3(0, 0, 1), true);
	AddMeshToList(p0);

	Mesh* p1 = Objects2D::CreateRectangle("left", corner1, 25, resolution.y - 80.0f, glm::vec3(1, 0, 0), true);
	AddMeshToList(p1);
	
	Mesh* p2 = Objects2D::CreateRectangle("up", corner2, resolution.x, 25.0f, glm::vec3(1, 0, 0), true);
	AddMeshToList(p2);

	Mesh* p3 = Objects2D::CreateRectangle("right", corner3, 25, resolution.y - 80.0f, glm::vec3(1, 0, 0), true);
	AddMeshToList(p3);

	Mesh* p4 = Objects2D::CreateRectangle("platform", element, length, width, glm::vec3(0, 1, 0), true);
	AddMeshToList(p4);

	Mesh* p5 = Objects2D::CreateBall("ball", element, radius, glm::vec3(1, 1, 1), true);
	AddMeshToList(p5);

	Mesh* p6 = Objects2D::CreateRectangle("powerup", element, squareSide, squareSide, glm::vec3(0, 0, 1), true);
	AddMeshToList(p6);

	float x = 120, y = 360;
	glm::vec3 color1 = glm::vec3(1, 0, 0), color2 = glm::vec3(1, 1, 0), coloraux;
	int life1 = 1, life2 = 2, lifeaux;
	Brick aux;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 15; j++) {
			 //Adaugam caramizile intr-un vector
			 aux.i = i;
			 aux.j = j;
			 aux.x = x;
			 aux.y = y;
			 aux.duritate = life1;
			 aux.scalat = 0;
			 aux.powerUp = rand() % 2;
			 bricks.push_back(aux);
			 
			 char buf[8] = "brick";
			 buf[5] = i + '0';
			 buf[6] = j + '0';
			 Mesh* p = Objects2D::CreateRectangle(buf, element, lengthBrick, widthBrick, color1, true);
			 AddMeshToList(p);			 
			 x += lengthBrick + 10;

			 //Schimba culoarea
			 coloraux = color1;
			 color1 = color2;
			 color2 = coloraux;
			 //Schimba numarul de vieti la caramida
			 lifeaux = life1;
			 life1 = life2;
			 life2 = lifeaux;
		}
		x = 120;
		y += widthBrick + 10;
	}
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Coliziune(float deltaTimeSeconds){
	//Ciocnirea cu peretele de jos
	if (translateY <= 10 + radius && PowerUpTime > 0) {
		translateSignY *= (-1);
	}

	//Ciocnirea cu peretele de sus
	if (translateY >= resolution.y - 25 - radius) {
		translateSignY *= (-1);
	}

	//Ciocnirea cu peretele stang
	if (translateX <= 25 + radius && translateY > 80 - radius) {
		translateSignX *= (-1);
	}

	//Ciocnirea cu peretele drept
	if (translateX >= resolution.x - 25 - radius && translateY > 80 - radius) {
		translateSignX *= (-1);
	}

	//Ciocnirea cu coltul stang
	if (0 <= translateX && translateX <= 25 + radius && 80 - radius < translateY && translateY < 80) {
		translateSignY *= (-1);
	}

	//Ciocnirea cu coltul drept
	if (resolution.x - 25 - radius <= translateX && translateX <= resolution.x && 
		80 - radius < translateY && translateY < 80) {
		translateSignY *= (-1);
	}

	//Ciocnirea cu platforma
	if (translateX >= platformaX && translateX <= platformaX + length &&
		translateY <= platformaY + width + radius && translateY > platformaY) {
		float pi = 3.14f;
		float angle = (platformaX + length - translateX) * pi / length;
		translateSignX = cos(angle);
		translateSignY = sin(angle);
	}

	//Ierirea mingi din joc
	if (translateY <= 0 - radius || translateX <= 0 - radius || resolution.x + radius <= translateX) {
		Mouse_pressed = false;
		NrVieti--;
		translateX = platformaX + length / 2;
		translateY = 60;
		translateSignX = 1;
		translateSignY = 1;
	}
}

void Tema1::ColiziuneCaramida(Brick *b){
		float x1 = b->x;
		float y1 = b->y;
		float x2 = b->x + lengthBrick;
		float y2 = b->y + widthBrick;
		int sem = 0;

		// Partea de jos a caramizii
		if (x1 - radius < translateX && translateX < x2 + radius && 
			y1 - radius < translateY && translateY < y1) {
			translateSignY *= (-1);
			sem = 1;
		}

		// Partea de sus a caramizii
		if (x1 - radius < translateX && translateX < x2 + radius &&
			y2 < translateY && translateY < y2 + radius) {
			translateSignY *= (-1);
			sem = 1;
		}

		// Partea stanga a caramizii
		if (x1 - radius < translateX && translateX < x1 && y1 < translateY && translateY < y2) {
			translateSignX *= (-1);
			sem = 1;
		}

		// Partea dreapta a caramizii
		if (x2 < translateX && translateX < x2 + radius && y1 < translateY && translateY < y2) {
			translateSignX *= (-1);
			sem = 1;
		}
		
		// Schimba culoarea caramizii in rosu sau va fi scalata
		if (sem == 1) {
			b->duritate--;
			if (b->duritate == 0) {
				b->scalat = 1;
			} else {
				glm::vec3 brick = glm::vec3(0, 0, 0);
				modelMatrix *= Transform2D::Translate(b->x, b->y);
				char buf[8] = "brick";
				buf[5] = b->i + '0';
				buf[6] = b->j + '0';
				Mesh* p = Objects2D::CreateRectangle(buf, brick, lengthBrick, widthBrick, glm::vec3(1, 0, 0), true);
				AddMeshToList(p);
			}
		}
}

void Tema1::Update(float deltaTimeSeconds)
{
	for (int i = 0; i < NrVieti; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(10.0f + i * 25.0f, 20.0f);
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	}

	for (int i = 0; i < bricks.size(); i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bricks[i].x, bricks[i].y);
		if (bricks[i].scalat == 0) {
			ColiziuneCaramida(&bricks[i]);
		}

		char buf[8] = "brick";
		buf[5] = bricks[i].i + '0';
		buf[6] = bricks[i].j + '0';

		if(bricks[i].scalat == 0){			
			RenderMesh2D(meshes[buf], shaders["VertexColor"], modelMatrix);
		}else {
			scaleX -= deltaTimeSeconds * 2;
			scaleY -= deltaTimeSeconds * 2;
			modelMatrix *= Transform2D::Translate(lengthBrick / 2, widthBrick / 2);
			modelMatrix *= Transform2D::Scale(scaleX, scaleY);
			modelMatrix *= Transform2D::Translate((-1) * lengthBrick / 2, (-1) * widthBrick / 2);
			RenderMesh2D(meshes[buf], shaders["VertexColor"], modelMatrix);

			if (scaleX <= 0 && scaleY <= 0) {
				if (bricks[i].powerUp == 1) {
					PowerUp pu;
					pu.TranslateX = bricks[i].x + lengthBrick/2 - squareSide / 2;
					pu.TranslateY = bricks[i].y + widthBrick/2 - squareSide / 2;
					powersUp.push_back(pu);
				}
				scaleX = 1;
				scaleY = 1;
				bricks.erase(bricks.begin() + i);				
			}
		}
	}

	modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["left"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["up"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["right"], shaders["VertexColor"], modelMatrix);

	modelMatrix *= Transform2D::Translate(platformaX, platformaY);
	RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	Coliziune(deltaTimeSeconds);
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	
	angularStep -= deltaTimeSeconds;
	PowerUpTime -= deltaTimeSeconds;
	for (int i = 0; i < powersUp.size(); i++) {
		modelMatrix = glm::mat3(1);
		powersUp[i].TranslateY -= deltaTimeSeconds * 100;
		modelMatrix *= Transform2D::Translate(powersUp[i].TranslateX, powersUp[i].TranslateY);
		modelMatrix *= Transform2D::Translate(squareSide / 2, squareSide / 2);
		modelMatrix *= Transform2D::Rotate(angularStep);
		modelMatrix *= Transform2D::Translate((-1) * squareSide / 2, (-1) * squareSide / 2);
		RenderMesh2D(meshes["powerup"], shaders["VertexColor"], modelMatrix);

		if (platformaX <= powersUp[i].TranslateX + squareSide/2 && powersUp[i].TranslateX + squareSide/2 <= platformaX + length &&
			platformaY <= powersUp[i].TranslateY + squareSide/2 && powersUp[i].TranslateY + squareSide/2 <= platformaY + width) {
			PowerUpTime = 30;
		}
	}

	if (PowerUpTime > 0) {
		modelMatrix = glm::mat3(1);
		RenderMesh2D(meshes["down"], shaders["VertexColor"], modelMatrix);
	}

	if (NrVieti == 0 || bricks.size() == 0) {
		Init();
	}
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (Mouse_pressed == true) {
			translateY += translateSignY * deltaTime * 250;
			translateX += translateSignX * deltaTime * 250;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && Mouse_pressed == false) {
		Mouse_pressed = true;
		translateSignX = 0;
	}

	platformaX = mouseX - length / 2;
	if (Mouse_pressed == false) {
		translateX = (float)mouseX;
	}
}
