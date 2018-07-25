#pragma once
#include"MPMParticle.h"
typedef struct ElasticParameters
{
	double Lambda;
	double Mu;
	double mass;
} ElasticParameters;

class MPMParticleElastic
	:public MPMParticle
{
public:
	MPMParticleElastic();
	MPMParticleElastic(Vector2d pos, Vector2d vel, double mass, int num, double lamda, double mu);
	MPMParticleElastic(Vector2d pos, Vector2d vel, int num, ElasticParameters p, Vector3d PColor);
	~MPMParticleElastic();
	double Lambda;
	double Mu;
	Matrix2d energyDerivative();
	void MPMUpdate();
	void DeltaForce();
	void updatePos();
	void updateGradient();
	void applyPlasticity();
};