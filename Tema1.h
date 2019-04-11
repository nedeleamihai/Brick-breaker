#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

struct Brick {
	int i;
	int j;
	float x;
	float y;
	int duritate;
	int scalat;
	int powerUp;
};

struct PowerUp {
	float TranslateX;
	float TranslateY;
};

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();
		bool Mouse_pressed;
		glm::ivec2 resolution;

		void Init() override;

	private:
		void FrameStart() override;
		void Coliziune(float deltaTimeSeconds);
		void ColiziuneCaramida(Brick *bri);
		void Update(float deltaTimeSeconds) override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

	protected:
		glm::mat3 modelMatrix;
		float width, length, radius, widthBrick, lengthBrick, squareSide;
		float translateX, translateY;
		float platformaX, platformaY;
		float translateSignX, translateSignY;
		float scaleX, scaleY;
		float angularStep;
		float PowerUpTime;
		int NrVieti;
		std::vector<PowerUp> powersUp;
		std::vector<Brick> bricks;
};
