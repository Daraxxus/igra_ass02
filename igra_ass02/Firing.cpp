#include "stdafx.h"
#include "Firing.h"
#include "Tank.h"
#include <vector>

#include <math.h>

std::vector<Firing*> Firing::shell;

Firing::Firing(float xSPos, float ySPos, float zSPos, float ySRot, float xSRotBarrel, float ySRotBarrel)
{
	speed = 5;
	gravity = -9.81; 
	yVel = 0;

	xRotBarrel = xSRotBarrel;
	yRotBarrel = ySRotBarrel;
	yRot = ySRot;
	bulletRotation = -yRotBarrel;
	deltaBulletRotation = 0;
	originalRotation = ySRotBarrel;
	originalY = ySPos;

	float radYRotBarrel = yRotBarrel * 3.14159 / 180;
	float radXRotBarrel = xRotBarrel * 3.14159 / 180;
	float radYRot = yRot * 3.14159 / 180;

	float dist = 2 * cos(-radYRotBarrel);
	yPos = ySPos + 1.5 + 2 * sin(-radYRotBarrel);
	zPos = zSPos + dist * cos(radYRot + radXRotBarrel);
	xPos = xSPos + dist * sin(radYRot + radXRotBarrel);

	maxHeightCurve = ((speed * speed) * (sin(-radYRotBarrel) * sin(-radYRotBarrel))) / -2 * gravity;

	forwardX = sin(radYRot + radXRotBarrel) * speed;
	forwardZ = cos(radYRot + radXRotBarrel) * speed;
	forwardY = sin(-radYRotBarrel) * speed;

	yVel = forwardY;
}

void Firing::DrawProjectile()
{
	//FiringPoint 
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos); //move the bullet to location
	glRotatef(180 + yRot + xRotBarrel, 0, 1, 0); //rotate bullet based on where barrel is pointing
	glRotatef(bulletRotation, 1, 0, 0); //rotate bullet based on angle of barrel and trajectory
	gluCylinder(gluNewQuadric(), 0.1, 0.2, 0.4, 32, 32);
	glPopMatrix();
}

void Firing::CalcAngleChange()
{
	float changeInY = maxHeightCurve - originalY;
	deltaBulletRotation = 90 / changeInY;
}


void Firing::Update(double deltaTime)
{
	/*x = power * cos(elevation) * sin(azimuth);
	z = power * cos(elevation) * cos(azimuth);
	y = power * sin(elevation);*/

	//CalcAngleChange();
	if (yPos < 0) {
		yPos = 0;
	}
	if (yPos != 0) {
		yVel += gravity * deltaTime;
		
		xPos += forwardX * deltaTime;
		zPos += forwardZ * deltaTime;
		yPos += yVel * deltaTime;
		bulletRotation += deltaBulletRotation;
	}
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
