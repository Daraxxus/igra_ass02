#include "stdafx.h"
#include "Firing.h"
#include "Tank.h"
#include <vector>

#include <math.h>

std::vector<Firing*> Firing::shell;

Firing::Firing(float xSPos, float ySPos, float zSPos)
{
	speed = 10;

	xPos = xSPos;
	yPos = ySPos;
	zPos = zSPos;

	forwardX = sin(Tank::Player.degToRad(Tank::Player.yRot + Tank::Player.yRotBarrel));
	//forwardY = sin(Tank::Player.degToRad(Tank::Player.xRotBarrel));
	forwardZ = cos(Tank::Player.degToRad(Tank::Player.yRot + Tank::Player.yRotBarrel));
}

void Firing::DrawProjectile()
{
	float angularStep = 360.0 / 36;
	float x, y = 0;
	double pi = 3.1415926535;

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(xPos, yPos, zPos);

	glRotatef(pRotY, 0, 1, 0);
	glRotatef(pRotZ, 0, 0, 1);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 0.5, 32, 32);

	glPopMatrix();

}

void Firing::CalcTraj()
{

}

void Firing::Update(double deltaTime)
{
	xPos += forwardX * speed * deltaTime;
	yPos = speed * forwardY * deltaTime - 0.5 * 9.81 * deltaTime * deltaTime;
	zPos += forwardZ * speed * deltaTime;
}

void Firing::HandleKeyDown()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		Firing *fire = new Firing(Tank::Player.xPos, Tank::Player.yPos + 1, Tank::Player.zPos);
		shell.push_back(fire);
	}
}

Firing::~Firing()
{
}
