#include "stdafx.h"
#include "Firing.h"
#include "Tank.h"
#include <vector>

#include <math.h>

std::vector<Firing*> Firing::shell;

Firing::Firing(float xSPos, float ySPos, float zSPos, float ySRot, float xSRotBarrel, float ySRotBarrel, AvailableSpeed chosenSpeed)
{
	switch (chosenSpeed) {
	case AvailableSpeed::SLOW:
		speed = 4;
		break;
	case AvailableSpeed::MEDIUM:
		speed = 6;
		break;
	case AvailableSpeed::FAST:
		speed = 8;
		break;
	}

	gravity = -9.81; 
	yVel = 0;
	x = 0;

	xRotBarrel = xSRotBarrel;
	yRotBarrel = ySRotBarrel;
	yRot = ySRot;
	bulletRotation = -yRotBarrel;
	deltaBulletRotation = 0;
	originalRotation = ySRotBarrel;
	originalY = ySPos;

	radYRotBarrel = yRotBarrel * 3.14159 / 180;
	radXRotBarrel = xRotBarrel * 3.14159 / 180;
	radYRot = yRot * 3.14159 / 180;

	float dist = 2 * cos(-radYRotBarrel);
	yPos = ySPos + 1.5 + 2 * sin(-radYRotBarrel);
	zPos = zSPos + dist * cos(radYRot + radXRotBarrel);
	xPos = xSPos + dist * sin(radYRot + radXRotBarrel);

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
	glTranslatef(xPos, yPos, zPos);				 //move the bullet to location
	glRotatef(180 + yRot + xRotBarrel, 0, 1, 0); //rotate bullet based on where barrel is pointing
	glRotatef(bulletRotation, 1, 0, 0);			 //rotate bullet based on angle of barrel and trajectory
	gluCylinder(gluNewQuadric(), 0.1, 0.2, 0.4, 32, 32);
	glPopMatrix();
}

void Firing::CalcAngleDuringTraj() //angle of bullet when following curve
{
	float Gradient = tan(-radYRotBarrel) - (-gravity * x) / (2 * speed * speed * cos(-radYRotBarrel) * cos(-radYRotBarrel));
	bulletRotation = atan(Gradient) * 180 / 3.14159;
}


void Firing::Update(double deltaTime)
{
	debug(to_string(speed));
	if (yPos < 0) {
		yPos = 0; //stop bullet on floor
	}

	if (yPos != 0) {
		yVel += gravity * deltaTime;
		
		xPos += forwardX * deltaTime;
		zPos += forwardZ * deltaTime;
		yPos += yVel * deltaTime;
		x += speed * deltaTime;
		CalcAngleDuringTraj();
	}
}

void Firing::HandleKeyDown(std::vector<float> GetPosRot , AvailableSpeed chosenSpeed)
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
	Firing *fire = new Firing(x, y, z, yR, xRB, yRB, chosenSpeed);
	shell.push_back(fire);
}

Firing::~Firing()
{
}
