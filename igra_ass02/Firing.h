#pragma once
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
class Firing
{
public:
	Firing(float xSPos, float ySPos, float zSPos);
	void DrawProjectile();
	void CalcTraj();

	void Update(double deltaTime);
	static void HandleKeyDown();
	static std::vector<Firing*> shell;

	~Firing();
private:
	bool isFired;

	float speed;
	float cd;
	float angleProj;

	float pRotY, pRotZ;

	float xPos, yPos, zPos;
	float forwardX, forwardY, forwardZ;
	float yRot, yRotBarrel;
};

