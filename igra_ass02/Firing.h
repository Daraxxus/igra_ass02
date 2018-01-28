#pragma once
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
class Firing
{
public:
	enum AvailableSpeed { SLOW, MEDIUM, FAST }; //change speed

	Firing(float xSPos, float ySPos, float zSPos, float ySRot, float xSRotBarrel, float ySRotBarrel, AvailableSpeed chosenSpeed);

	void DrawProjectile();
	void CalcAngleDuringTraj();
	void Update(double deltaTime);
	static void HandleKeyDown(std::vector<float> GetPosRot, AvailableSpeed chosenSpeed);
	static std::vector<Firing*> shell;
	~Firing();

private:
	bool isFired;

	float speed, gravity;
	float cd;
	float angleProj;

	float maxHeightCurve;
	float bulletRotation, deltaBulletRotation;
	float originalY, originalRotation;

	float xPos, yPos, zPos;
	float forwardX, forwardY, forwardZ;
	float yRot, yRotBarrel, xRotBarrel;
	float radYRotBarrel, radXRotBarrel, radYRot;
	float x;

	float yVel;
};

