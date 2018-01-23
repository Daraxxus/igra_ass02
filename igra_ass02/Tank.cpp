#include "stdafx.h"
#include "Tank.h"

#include <math.h>

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

Tank Tank::Player;

Tank::Tank()
{
	width = 3.0f;
	length = 1.5f;
	height = 4.0f;
}

float tankVertices[][3] = {
	{ 0, 0, 0 }, //0
	{ -3, 0, 0 }, //1
	{ -3, 1, 0 }, //2
	{ 0, 1, 0 }, //3
	{ 0, 0, -2 }, //4
	{ 0, 1, -2 }, //5 
	{-3, 1, -2 }, //6
	{ -3, 0, -2 }, //7 //bottom half

	{-0.5, 1, -0.5}, //8
	{-2.5, 1, -0.5}, //9
	{-2.5, 2 , -0.5}, //10
	{-0.5, 2, -0.5}, //11

	{-0.5, 1, -1.5}, //12
	{-0.5, 2, -1.5}, //13

	{-2.5, 1, -1.5}, //14
	{-2.5, 2, -1.5}  //15
};

int tankIndices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	4, 0, 3, 5,
	2, 1, 7, 6,
	3, 2, 6, 5,
	4, 7, 1, 0, //upperhalf

	8, 9, 10, 11,
	12, 13, 15, 14,
	12, 8, 11, 13,
	10, 9, 14, 15,
	11, 10, 15, 13,
	12, 14, 9, 8 //bottomhalf
	
};

float tankColors[][3] = {
	0, 1, 0, // Front is Green
	1, 0, 0, // Back is Red
	0, 0, 1, // Right is Blue
	1, 1, 0, // Left is Yellow
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,

	0, 1, 0, // Front is Green
	1, 0, 0, // Back is Red
	0, 0, 1, // Right is Blue
	1, 1, 0, // Left is Yellow
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5
};

void Tank::DrawTank() {
	float angularStep = 360.0 / 36;
	float x, y = 0;
	double pi = 3.1415926535;
	
	
	glPushMatrix();
	float hl = 2 * 0.5f;
	float a = 0.0f;
	float step = (float)3.1415926535 / (float)180;
	
	glTranslatef(xPos, yPos, zPos);
	glRotatef(yRot, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW, transform);

	glPushMatrix();
	
	glTranslatef(1, 0, -1);
	glRotatef(90, 0, 1, 0);
	glColor3f(0.0, 0.0, 0.0);

	int index = 0;

	for (int qd = 0; qd < 12; qd++) { //Draw Bottom of Tank
		glBegin(GL_QUADS);
		glColor3f(tankColors[qd][0], tankColors[qd][1],
			tankColors[qd][2]);
		for (int v = 0; v < 4; v++) { //Draw Top of Tank
			glVertex3f(tankVertices[tankIndices[index]][0],
				tankVertices[tankIndices[index]][1],
				tankVertices[tankIndices[index]][2]);
			index++;
		}
		glEnd();
	}
	glPopMatrix();

	//Draw Barrel of Tank
	glRotatef(180, 0, 1, 0);

	glColor3f(0, 1, 0);
	glTranslatef(0, 1.5, 0);

	glRotatef(yRotBarrel, 0, 1, 0);	
	glRotatef(xRotBarrel, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 3, 32, 32);

	glPopMatrix();
}

void Tank::HandleKeyDown(double deltaTime)
{
	glPushMatrix();

	//Tank Movement
	if (GetAsyncKeyState(VK_A))
	{
		Rotate(5);
	}
	if (GetAsyncKeyState(VK_D))
	{
		Rotate(-5);
	}
	if (GetAsyncKeyState(VK_W))
	{
		MoveForward(2 * deltaTime);
	}
	if (GetAsyncKeyState(VK_S))
	{
		MoveForward(-2 * deltaTime);
	}
	
	//Barrel Movement
	if (GetAsyncKeyState(VK_LEFT))
	{
		yRotBarrel += 1;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		yRotBarrel -= 1;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		xRotBarrel -= 1;
		if (xRotBarrel < -45)
		{
			xRotBarrel = -45;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		xRotBarrel += 1;
		if (xRotBarrel > 0)
		{
			xRotBarrel = 0;
		}
	}

	glPopMatrix();
}

float Tank::degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise! 
	return degAngle / 180.0 * pi;
}

void Tank::MoveForward(double dist)
{
	// Movement must be based on orientation of player
	double deltaX = 0;
	double deltaZ = 0;
	// Calculate translation as based on current yRotation angle
	deltaX = -dist*sin(degToRad(yRot));
	deltaZ = -dist*cos(degToRad(yRot));
	// Update the position
	xPos = xPos + deltaX;
	zPos = zPos + deltaZ;
}

void Tank::Rotate(double angle)
{
	// Update the rotation (yaw)
	yRot = yRot + angle;
}

void Tank::UpwardVector(double dist)
{
	double deltaY = 0;

	deltaY = -dist*sin(degToRad(xRot));

	yPos = yPos + deltaY;
}

Tank::~Tank()
{
}
