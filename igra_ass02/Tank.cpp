#include "stdafx.h"
#include "Tank.h"

#include <math.h>

Tank Tank::Player;

Tank::Tank()
{
	xPos = 0;
	yPos = 0;
	zPos = 0;
	maxSpeed = 2;
	acceleration = 0.01;
	speed = 0;
	drag = -0.01;
	dirX = 0;
	dirZ = 0;
	movingW = false;
	movingS = false;
}

float tankVertices[][3] = {
	{ 1, 0, 1 }, //0
	{-1, 0, 1 }, //1
	{-1, 1, 1 }, //2
	{ 1, 1, 1 }, //3
	{ 1, 0, -1 }, //4
	{ 1, 1, -1 }, //5 
	{-1, 1, -1 }, //6
	{-1, 0, -1 }, //7 //bottom half
};

int tankIndices[] = {
	0, 1, 2, 3, //left face
	4, 5, 6, 7, //right face
	4, 0, 3, 5, //front face
	2, 1, 7, 6, //back face
	3, 2, 6, 5, //top face
	4, 7, 1, 0, //bottom face
};

float tankColors[][3] = {
	0, 1, 0, // Front is Green
	1, 0, 0, // Back is Red
	0, 0, 1, // Right is Blue
	1, 1, 0, // Left is Yellow
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
};

void Tank::DrawTank() {
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(yRot, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW, transform);
	int index = 0;

	for (int qd = 0; qd < 6; qd++) {
		glBegin(GL_QUADS);
		glColor3f(tankColors[qd][0], tankColors[qd][1],
			tankColors[qd][2]);
		for (int v = 0; v < 4; v++) { 
			glVertex3f(tankVertices[tankIndices[index]][0],
				tankVertices[tankIndices[index]][1],
				tankVertices[tankIndices[index]][2]);
			index++;
		}
		glEnd();
	}

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, 1, 0);
	GLUquadric *quad;
	quad = gluNewQuadric();
	gluSphere(quad, 1, 100, 20);
	glPopMatrix();

	//Draw Barrel of Tank
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(0, 1.5, 0);

	glRotatef(xRotBarrel, 0, 1, 0);	
	glRotatef(yRotBarrel, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 2, 32, 32);
	glPopMatrix();
	glPopMatrix();

	//FiringPoint 
	glColor3f(0, 0, 1);
	glPushMatrix();

	float radYRotBarrel = degToRad(yRotBarrel);
	float radXRotBarrel = degToRad(xRotBarrel);
	float radYRot = degToRad(yRot);
	float dist = zPos + 2 * cos(-radYRotBarrel); 
	float y = yPos + 1.5 + 2 * sin(-radYRotBarrel);

	float z = zPos + dist * cos(radYRot + radXRotBarrel);
	float x = xPos + dist * sin(radYRot + radXRotBarrel);
	
	/*debug("x: " + to_string(x) + " y: " + to_string(y) + " z: " + to_string(z) + " dist: " + to_string(dist) + " xPos: " + to_string(xPos) + " yPos: " + to_string(yPos) + " zPos: " + to_string(zPos) + " angle: " + to_string(xRotBarrel));*/

	glTranslatef(x, y, z);

	GLUquadric *quadtest;
	quadtest = gluNewQuadric();
	gluSphere(quadtest, 0.25, 100, 20);
	glPopMatrix();
}


void Tank::HandleKeyDown(double deltaTime)
{
	//Tank Movement
	if (GetAsyncKeyState(VK_A))
	{
		Rotate(-20 * deltaTime);
	}
	if (GetAsyncKeyState(VK_D))
	{
		Rotate(20 * deltaTime);
	}
	if (GetAsyncKeyState(VK_W))
	{
		movingW = true;
	}
	if (GetAsyncKeyState(VK_S))
	{
		movingS = true;
	}
	
	//Barrel Movement
	if (GetAsyncKeyState(VK_LEFT))
	{
		xRotBarrel -= 0.1;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		xRotBarrel += 0.1;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		yRotBarrel -= 0.1;
		if (yRotBarrel < -45)
		{
			yRotBarrel = -45;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		yRotBarrel += 0.1;

		if (yRotBarrel > 0)
		{
			yRotBarrel = 0;
		}
	}
}

void Tank::Update(double deltaTime) {
	HandleKeyDown(deltaTime);
	if (movingS || movingW) {
		if (movingW) {
			MoveForward(acceleration, deltaTime);
		}

		if (movingS) {
			MoveForward(-acceleration, deltaTime);
		}
	}

	if (!GetAsyncKeyState(VK_W) && !GetAsyncKeyState(VK_S)) {
		if (!GetAsyncKeyState(VK_W)) {
			movingW = false;
		}

		if (!GetAsyncKeyState(VK_S)) {
			movingS = false;
		}
		if (speed > 0) {
			speed += drag * deltaTime;
			if (speed <= 0) {
				speed = 0;
			}
		}
		if (speed < 0) {
			speed -= drag * deltaTime;
			if (speed >= 0) {
				speed = 0;
			}
		}
	}

	// Update the position
	xPos = xPos + speed * dirX;
	zPos = zPos + speed * dirZ;
	debug("speed: " + to_string(speed));
}

float Tank::degToRad(float degAngle) {
	double pi = 3.1415926535;
	return degAngle / 180.0 * pi;
}

void Tank::MoveForward(float acceleration, double deltaTime)
{
	// Calculate direction as based on current yRotation angle
	dirX = sin(degToRad(yRot));
	dirZ = cos(degToRad(yRot));
	if (fabsf(speed) < maxSpeed) {
		speed += acceleration * deltaTime;
	}
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

std::vector<float> Tank::ReturnCurrentPosition() {
	std::vector<float> currentPosRot = { xPos, yPos, zPos, yRot, xRotBarrel, yRotBarrel };
	return currentPosRot;
}

Tank::~Tank()
{
}
