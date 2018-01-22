#include "stdafx.h"
#include "Firing.h"

float projRotX, projRotY, projRotZ;

Firing::Firing()
{
}

void Firing::DrawProjectile()
{

}

void Firing::CalcTraj()
{

}

void Firing::Update(double deltaTime)
{
	this->deltaTime = deltaTime;
	projRotY = deltaTime * projSpeed;
}


Firing::~Firing()
{
}
