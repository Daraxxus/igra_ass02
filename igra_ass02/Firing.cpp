#include "stdafx.h"
#include "Firing.h"
#include "Tank.h"
#include <vector>

#include <math.h>

std::vector<Firing*> Firing::shell;

Firing::Firing(float xSPos, float ySPos, float zSPos, float ySRot, float xSRotBarrel, float ySRotBarrel)
{
	speed = 10;

	/*xPos = xSPos;
	yPos = ySPos + 1.5;
	zPos = zSPos;
	yRot = ySRot;*/
	xRotBarrel = xSRotBarrel;
	yRotBarrel = ySRotBarrel;

	float dist = xSPos + 3 * cos(xRotBarrel);
	yPos = 3 * sin(xRotBarrel);
	xPos = xSPos + dist * cos(yRotBarrel);
	zPos = zSPos + dist * sin(yRotBarrel);

	forwardX = sin(Tank::Player.degToRad(Tank::Player.yRot + Tank::Player.yRotBarrel));
	//forwardY = sin(Tank::Player.degToRad(Tank::Player.xRotBarrel));
	forwardZ = -cos(Tank::Player.degToRad(Tank::Player.yRot + Tank::Player.yRotBarrel));
}

void Firing::DrawProjectile()
{
	glColor3f(1, 0, 0);
	glPushMatrix();
	/*glRotatef(yRot, 0, 1, 0);
	glRotatef(yRotBarrel, 0, 1, 0);
	glRotatef(-xRotBarrel, 1, 0, 0);*/
	glTranslatef(xPos, yPos, zPos);

	gluCylinder(gluNewQuadric(), 0.1, 0.2, 0.5, 32, 32);
	glPopMatrix();
}

void Firing::CalcTraj()
{

}

void Firing::Update(double deltaTime)
{
	xPos += forwardX * speed * deltaTime;
	//yPos = speed * forwardY * deltaTime - 0.5 * 9.81 * deltaTime * deltaTime;
	zPos += forwardZ * speed * deltaTime;
}

void Firing::HandleKeyDown(std::vector<float> GetPosRot)
{
	float x, y, z, yR, xRB, yRB;
	for (int i = 0; i < GetPosRot.size(); i++) {
		switch (i) {
		case 0: x = GetPosRot[i];
			break;
		case 1: y = GetPosRot[i];
			break;
		case 2: z = GetPosRot[i];
			break;
		case 3: yR = GetPosRot[i];
			break;
		case 4: xRB = GetPosRot[i];
			break;
		case 5: yRB = GetPosRot[i];
			break;
		}
	}
	Firing *fire = new Firing(x, y, z, yR, xRB, yRB);
	shell.push_back(fire);
}

Firing::~Firing()
{
}
