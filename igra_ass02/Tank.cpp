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

void Tank::CalculateNoramal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float &normalX, float &normalY, float &normalZ)
{
	float tempX, tempY, tempZ;

	tempX = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	tempY = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	tempZ = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	Normalize(tempX, tempY, tempZ, normalX, normalY, normalZ);
}

void Tank::FindMidPoint(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float &midpointX, float &midpointY, float &midpointZ)
{

	midpointX = (x1 + x2 + x3) / 3;
	midpointY = (y1 + y2 + y3) / 3;
	midpointZ = (z1 + z2 + z3) / 3;
}

void Tank::Normalize(float b4NormalX, float b4NormalY, float b4NormalZ, float &_normalX, float &_normalY, float &_normalZ)
{
	float vectorLength = sqrtf(powf(b4NormalX, 2) + powf(b4NormalY, 2) + powf(b4NormalZ, 2));
	_normalX = b4NormalX / vectorLength;
	_normalY = b4NormalY / vectorLength;
	_normalZ = b4NormalZ / vectorLength;
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

//float tankColors[][3] = {
//	0, 1, 0, // Front is Green
//	1, 0, 0, // Back is Red
//	0, 0, 1, // Right is Blue
//	1, 1, 0, // Left is Yellow
//	0.5, 0.5, 0.5,
//	0.5, 0.5, 0.5,
//};

void Tank::DrawTank() {
	glEnable(GL_LIGHTING);

	Material redMaterial =
	{
		{ 0.7, 0.0, 0.0, 1.0 }, // Ambient
		{ 0.7, 0.0, 0.0, 1.0 }, // Diffuse
		{ 0.8, 0.8, 0.8, 1.0 }, // Specular
		32 // Shininess
	};

	Material blueMaterial = 
	{
		{ 0.0, 0.0, 0.7, 1.0 }, // Ambient
		{ 0.0, 0.0, 0.9, 1.0 }, // Diffuse
		{ 0.8, 0.8, 0.8, 1.0 }, // Specular
		32 // Shininess
	};

	Material blackMaterial = 
	{
		{ 0.0, 0.0, 0.0, 1.0 }, // Ambient
		{ 0.0, 0.0, 0.0, 1.0 }, // Diffuse
		{ 0.8, 0.8, 0.8, 1.0 }, // Specular
		32 // Shininess
	};

	float currentNormalX;
	float currentNormalY;
	float currentNormalZ;

	float currentMidPointX;
	float currentMidPointY;
	float currentMidPointZ;

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(yRot, 0, 1, 0);
	int index = 0;
	float tempArray[3][3];
	int tempIndex = 0;

	SetThisMaterial(&blueMaterial);
	for (int qd = 0; qd < 6; qd++) {
		glBegin(GL_QUADS);
		for (int v = 0; v < 4; v++) { 
			glVertex3f(tankVertices[tankIndices[index]][0],
				tankVertices[tankIndices[index]][1],
				tankVertices[tankIndices[index]][2]);
			index++;
		}
		CalculateNoramal(tempArray[0][0], tempArray[0][1], tempArray[0][2],
			tempArray[1][0], tempArray[1][1], tempArray[1][2],
			tempArray[2][0], tempArray[2][1], tempArray[2][2],
			currentNormalX, currentNormalY, currentNormalZ);

		glNormal3f(-currentNormalX, -currentNormalY, -currentNormalZ);
		glEnd();
	}


	SetThisMaterial(&redMaterial);
	glPushMatrix();
	glTranslatef(0, 1, 0);
	GLUquadric *quad;
	quad = gluNewQuadric();
	gluSphere(quad, 1, 100, 20);
	glPopMatrix();

	//Draw Barrel of Tank
	SetThisMaterial(&blackMaterial);
	glPushMatrix();
	glTranslatef(0, 1.5, 0);

	glRotatef(xRotBarrel, 0, 1, 0);	
	glRotatef(yRotBarrel, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.3, 0.3, 2, 32, 32);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_LIGHTING);
}


void Tank::HandleKeyDown(double deltaTime)
{
	//Tank Movement
	if (GetAsyncKeyState(VK_A))
	{
		Rotate(25 * deltaTime);
	}
	if (GetAsyncKeyState(VK_D))
	{
		Rotate(-25 * deltaTime);
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
		xRotBarrel += 0.5;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		xRotBarrel -= 0.5;
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

void Tank::SetThisMaterial(Material *m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}

Tank::~Tank()
{
}
