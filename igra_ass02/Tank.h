#pragma once
#include <gl\gl.h>
#include <gl\glu.h>
class Tank
{
public:
	Tank();
	void DrawTank();
	void HandleKeyDown(WPARAM wParam);

	void MoveForward(double dist);
	void Rotate(double angle);

	void ElevationRotate(double eRotate);
	void SideRotate(double sRotate);

	float degToRad(float degAngle);

	GLfloat transform[16];
	~Tank();

private:
	float width, height, length;
};

