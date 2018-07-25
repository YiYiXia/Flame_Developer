#pragma once
#include"MPMParticle.h"
typedef struct WaterParameters
{
	double Gama;
	double Hardness;
	double mass;
}WaterParameters;

class MPMParticleWater
	:public MPMParticle 
{
public:
	MPMParticleWater();
	MPMParticleWater(Vector2d pos, Vector2d vel, double mass, int num, double gama, double hardness);
	MPMParticleWater(Vector2d pos, Vector2d vel, int num, WaterParameters p, Vector3d PColor);
	~MPMParticleWater();
	double J;
	double Gama;
	double Hardness;
	void updatePos();
	void updateGradient();
	Matrix2d energyDerivative();
	void MPMUpdate();
	void DeltaForce();
};