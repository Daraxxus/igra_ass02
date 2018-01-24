#pragma once
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
class Firing
{
public:
	Firing(float xSPos, float ySPos, float zSPos, float ySRot, float xSRotBarrel, float ySRotBarrel);
	void DrawProjectile();
	void CalcTraj();

	void Update(double deltaTime);
	static void HandleKeyDown(std::vector<float> GetPosRot);
	static std::vector<Firing*> shell;

	~Firing();
private:
	bool isFired;

	float speed;
	float cd;
	float angleProj;

	float xPos, yPos, zPos;
	float forwardX, forwardY, forwardZ;
	float yRot, yRotBarrel, xRotBarrel;
};

