#pragma once
#include <gl\GL.h>
#include <gl\GLU.h>
class Firing
{
public:
	Firing();
	void DrawProjectile();
	void CalcTraj();
	void Update(double deltaTime);
	~Firing();
private:
	bool isFired;

	float speed;
	float cd;
	float angleProj;

	float deltaTime;
	float projSpeed;
};

