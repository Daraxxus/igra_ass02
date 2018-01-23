#pragma once
#include <gl\gl.h>
#include <gl\glu.h>
class Tank
{
public:
	Tank();
	void DrawTank();
	void HandleKeyDown(double deltaTime);

	void MoveForward(double dist);
	void Rotate(double angle);

	float degToRad(float degAngle);

	GLfloat transform[16];

	void UpwardVector(double dist);

	float xPos, yPos, zPos;
	float xForwardV, yForwardV, zForwardV;
	float yRot;
	float xRot;
	float xRotBarrel, yRotBarrel;

	static Tank Player;

	~Tank();

private:
	float width, height, length;
};

