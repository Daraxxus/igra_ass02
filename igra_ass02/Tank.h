#pragma once
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

class Tank
{
public:
	Tank();
	void DrawTank();
	void HandleKeyDown(double deltaTime);
	void Update(double deltaTime);
	std::vector<float> ReturnCurrentPosition();

	void MoveForward(float acceleration, double deltaTime);
	void Rotate(double angle);

	float degToRad(float degAngle);

	GLfloat transform[16];

	void UpwardVector(double dist);

	float xPos, yPos, zPos;
	float speed, acceleration, drag, maxSpeed;
	float yRot;
	float xRot;
	float xRotBarrel, yRotBarrel;
	double dirX;
	double dirZ;

	static Tank Player;
	~Tank();

private:
	float width, height, length;
	bool movingW, movingS;
};

