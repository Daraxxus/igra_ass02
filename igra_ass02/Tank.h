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

	void SetThisMaterial(Material *m);

	void CalculateNoramal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3,
		float &normalX, float &normalY, float &normalZ);
	void FindMidPoint(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3,
		float &midpointX, float &midpointY, float &midpointZ);
	void Normalize(float b4NormalX, float b4NormalY, float b4NormalZ, float &_normalX, float &_normalY, float &_normalZ);

	static Tank Player;
	~Tank();

private:
	float width, height, length;
	bool movingW, movingS;
};

